#!/usr/bin/ruby

#这段写给你看的所以用中文，如果后面改的话记得删掉
#这几个是正则表达式，后面用于批量扫描的，后面会细说
dwarf_file_reg = /^file_names\[ *(?<num>\d+)\]:\n +name: \"(?<name>.+\.[ch]{1})\"/
dwarf_line_reg = /^0x0*(?<addr>[0-9a-f]+) +(?<line>\d+) +(?<col>\d+) +(?<file>\d+) +(?<isa>\d+) +(?<dis>\d+) +(?<flags>.+)/
objdump_func_reg = /^0*([0-9a-f]+) <(\S+)>:/
objdump_asm_reg = /^    (?<addr>[0-9a-f]+):\t(?<bin>(?: *[0-9a-f]{2})+) *\t(?<ins>([a-z]+ *[a-zA-Z0-9$,\%_\-\(\)\# \*\<\>\.\:\@\+\/\\]*)+)/

#读取文件地址，后面是抓去目录地址
#应该没啥问题，我也不算很确定，可以再de一下
path = "#{ARGV[0]}"
dir_path = path.gsub(/\/[^\/]*(?:\/)?$/, '')

#跑命令不多说
dwarf_raw = %x(llvm-dwarfdump --debug-line #{path})
objdump_raw = %x(objdump -d #{path})

#观察源文件用的姑且先注释一下
# File.open("dwarfdump.txt", 'w') { |f| f.write(dwarf_raw) }
# File.open("objdumpd.txt", 'w') { |f| f.write(objdump_raw) }

# 扫描匹配正则表达式
# 以下所有的数据类型都是数组，区别只是里面的小数组会有些差异
# [文件编码， 文件名] e.g. ["1", "merge1.c"]
dwarf_file = dwarf_raw.scan(dwarf_file_reg)
# 直接扒的dwarf的表，一一对应 e.g. ["11a9", "9", "1", "1", "0", "0", "is_stmt"]
dwarf_line = dwarf_raw.scan(dwarf_line_reg)
# 只读函数的部分，[地址， 函数名] e.g. ["1000", "_init"]
objdump_func = objdump_raw.scan(objdump_func_reg)
# 只读汇编的部分, [地址， 二进制， 指令] e.g. ["1000", "f3 0f 1e fa", "endbr64 "]
objdump_asm = objdump_raw.scan(objdump_asm_reg)

# 把函数的部分读成一个哈希表方便后面匹配
objdump_func_hash = Hash.new

objdump_func.each { |func|
    objdump_func_hash[func[0]] = func
}

# 以下变量分别是用来记录，读取出来的文件，C代码的行标记，下一个被标记的dwarf汇编行，分出来的块，当前编辑的块, 和新文件标注
files = Array.new(dwarf_file.length)
currCTarget = 0
currAsmTarget = 0
blocks = Array.new
currBlock = [Array.new, Array.new]
newFileFlag = nil

# 以汇编代码为基础过循环
objdump_asm.each{ |line|
    # 取得当前标记的汇编行
    asmTarget = dwarf_line[currAsmTarget]

    # 如果匹配上dwarf汇编行
    if (asmTarget && (asmTarget[0] == line[0]))
        # 对应的文件index先算出来
        file_idx = asmTarget[3].to_i(10)-1
        # 空的的话去读取文件
        if !(files[file_idx])
            newFileFlag = dwarf_file[file_idx]

            if dir_path == path
                cFile = File.open(newFileFlag[1], "r")
            else
                cFile = File.open(dir_path + "/" + newFileFlag[1], "r")
            end

            files[file_idx] = cFile.readlines.map(&:chomp)
            # 这个地方我不确定，目测文件如果分的话是会一块一块来然后跳转的，所以会重置C代码的标记行
            currCTarget = 0
            cFile.close
        end
        
        # 抓一下要分块的地方，然后把老的block给推进blocks里
        newTarget = asmTarget[1].to_i
        blocks.push(currBlock)

        # 三种情况新的代码推进了，就更新C代码块
        if newTarget > currCTarget
            currBlock = [files[file_idx][currCTarget..newTarget-1], Array.new]
            currCTarget = newTarget
        # 没推进就建个空的意思一下
        elsif newTarget == currCTarget
            currBlock = [Array.new, Array.new]
        # 如果发现是补全之前的，加个标注推进去
        else
            currBlock = [["* End of: "+files[file_idx][newTarget-1]], Array.new]
        end
        # 进入下一个dwarf汇编行
        currAsmTarget += 1
    end
    
    # 如果发现是新文件
    if newFileFlag
        currBlock[1].push(newFileFlag)
        newFileFlag = nil
    end

    # 如果发现这行还是个函数开头，先把函数信息给塞进去
    if (asmTarget && (objdump_func_hash[line[0]]))
        currBlock[1].push(objdump_func_hash[line[0]])
    end

    # 摆摆摆，恩摆😋，塞就完事了
    currBlock[1].push(line)
}
# 把最后的信息塞进去当最后一块
blocks.push(currBlock)

# 最后实际上blocks就是个长数组，每个block是个小pair，前面是C，后面是汇编的信息
puts blocks[1]