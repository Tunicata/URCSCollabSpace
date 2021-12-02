#!/usr/bin/ruby
require 'erb'

dwarf_file_reg = /^file_names\[ *(?<num>\d+)\]:\n +name: \"(?<name>.+\.[ch]{1})\"/
dwarf_line_reg = /^0x0*(?<addr>[0-9a-f]+) +(?<line>\d+) +(?<col>\d+) +(?<file>\d+) +(?<isa>\d+) +(?<dis>\d+) +(?<flags>.+)/
objdump_func_reg = /^0*([0-9a-f]+) <(\S+)>:/
objdump_asm_reg = /^    (?<addr>[0-9a-f]+):\t(?<bin>(?: *[0-9a-f]{2})+) *\t(?<ins>([a-z]+ *[a-zA-Z0-9$,\%_\-\(\)\# \*\<\>\.\:\@\+\/\\]*)+)/
objdump_callq_lit = /^(callq|jmp|bnd jmpq|jne|je|jb|ja|jg|jge|jbe|jae|jle|jl)\s+(?<addr>\h+).*/

path = ARGV[0]
dir_path = path.gsub(/\/[^\/]*(?:\/)?$/, '')

dwarf_raw = %x(llvm-dwarfdump --debug-line #{path})
objdump_raw = %x(objdump -d #{path})
sys_info = %x(uname -s -n -m -p -i -o)
sys_date = %x(date)

dwarf_file = dwarf_raw.scan(dwarf_file_reg)
dwarf_line = dwarf_raw.scan(dwarf_line_reg)
objdump_func = objdump_raw.scan(objdump_func_reg)
objdump_asm = objdump_raw.scan(objdump_asm_reg)

objdump_func_hash = Hash.new

objdump_func.each { |func|
    objdump_func_hash[func[0]] = func
}

files = Array.new(dwarf_file.length)
currCTarget = 0
currAsmTarget = 0
blocks = Array.new
currBlock = [Array.new, Array.new]
newFileFlag = nil

objdump_asm.each{ |line|
    asmTarget = dwarf_line[currAsmTarget]

    if (asmTarget && (asmTarget[0] == line[0]))
        file_idx = asmTarget[3].to_i(10)-1
        if !(files[file_idx])
            newFileFlag = dwarf_file[file_idx]

            if dir_path == path
                cFile = File.open(newFileFlag[1], "r")
            else
                cFile = File.open(dir_path + "/" + newFileFlag[1], "r")
            end

            files[file_idx] = cFile.readlines.map(&:chomp)
            currCTarget = 0
            cFile.close
        end
        
        newTarget = asmTarget[1].to_i
        blocks.push(currBlock)

        if newTarget > currCTarget
            currBlock = [files[file_idx][currCTarget..newTarget-1], Array.new]
            currCTarget = newTarget
        elsif newTarget == currCTarget
            currBlock = [Array.new, Array.new]
        else
            currBlock = [['// End of:' + files[file_idx][newTarget-1]], Array.new]
        end
        currAsmTarget += 1
    end
    
    if newFileFlag
        currBlock[1].push("----------------- file " + newFileFlag[0] + ": " + newFileFlag[1] + " -------------------\n")
        newFileFlag = nil
    end

    if (asmTarget && (objdump_func_hash[line[0]]))
        func = objdump_func_hash[line[0]]
        currBlock[1].push("000000000000" + func[0] + " &#60;" + func[1] + "&#62;:")
    end
    if line[2].match?(objdump_callq_lit)
        call_addr = line[2].match(objdump_callq_lit)[:addr]
        currBlock[1].push("<p id=\"t#{line[0]}\"><a href=\"#t#{call_addr}\">#{line[0]}:    #{line[1]}   #{line[2]}</a></p>")
    else
        currBlock[1].push("<p id=\"t#{line[0]}\">#{line[0]}:    #{line[1]}   #{line[2]}</p>")
    end
}
blocks.push(currBlock)

template = %q{
<html>
<script src="https://cdn.jsdelivr.net/gh/google/code-prettify@master/loader/run_prettify.js"></script>
<head>
    <title>disassem result - <%= ARGV[1] %></title>
    <style>
        th {
            vertical-align: top;
            text-align: left;
            border: 2px solid;
        }
        p {
            margin: 0px;
        }
        .prettyprint {
            border: none !important;
            padding: 0px !important;
        }
    </style>
</head>
<body>
<h2>disassem result - <%= path %></h2>
<h3>platform info:  <%= sys_info %>  </h3>
<h3>disassem time:  <%= sys_date %>  </h3>
<table>
    <% blocks.each do |b| %>
        <tr>
            <th>
                <pre class="prettyprint lang-c">
                    <% b[0].each do |s| %> <%= "\n"+ s.gsub('<','&#60;')%> <br /> <% end %>
                </pre>
            </th>
            <th>
                <% b[1].each do |s| %> 
                    <%= s %>
                <% end %>
            </th>
        </tr>
    <% end %>
</table>
</body>
</html>
}

template = ERB.new(template)
output = File.open(path + ".html", "w")
output.write(template.result(binding))

# For the sake of clarity, you should print the second and subsequent occurrences in a “grayed-out” color.  Vertical white space should be inserted as needed to make the alignment work out.