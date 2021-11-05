#!/usr/bin/ruby
require 'erb'

# Class Definitions

class Lbl
    String @data
    def data
        return @data
    end
    def initialize( data )
        @data = data
    end
end

class Asm
    String  @data
    Integer @index=0
    String  @asm
    def index
        return @index
    end
    def initialize( orig, index, asm )
        @data  = orig
        @index = Integer("0x#{index}")
        @asm   = asm
    end
    def to_string
        return "#{@index.to_s(16)}:   #{@asm}"
    end
end

class Block
    Array @src
    Array @asm
    def asm
        return @asm
    end
    def src
        return @src
    end
    def initialize( src, asm )
        @src = src
        @asm = asm
    end
end

# Extract Executable Information
path = "#{ARGV[0]}/#{ARGV[1]}"
dwarf_info = %x(llvm-dwarfdump --debug-line #{path}).split("\n")
objdump_info = %x(objdump -d #{path}).split("\n")

objs = Array.new
dwarfs = Array.new
filenames = Array.new
files = {}

for i in objdump_info do
    if i.size > 0 then
        if i[0] == "0" then 
            objs.push(Lbl.new(i[..-2]))
        else
            m = i.match /\s+(\h+):\s+[^\t]*\t(.*)/
            if (! m.nil?) then
                objs.push(Asm.new(i, m[1], m[2]))
            end
        end
    end
end

for i in dwarf_info do
    m = i.match /(0x\h+)\s+(\d+)\s+(\d+).*is_stmt ?([ \w]*)/
    if (! m.nil?) then
        dwarfs.push(m)
    end
    m = i.match /\s+name: "([^"]+)"/
    if (! m.nil?) then
        filenames.push([filenames.size + 1, m[1]])
    end
end

fs = Dir.entries(ARGV[0])

for f in filenames do
    if fs.include?(f[1]) then
        v = File.open("#{ARGV[0]}/#{f[1]}")
        files[f[0]] = v.readlines.map(&:chomp)
        v.close
    end
end

# Produce the Correspondence

blocks = Array.new
index = 0

obj_block = Array.new
addr0 = Integer(dwarfs[0][1])
while (objs[index].is_a? Lbl) or (objs[index].index < addr0) do
    if objs[index].is_a? Lbl then
        obj_block.push(objs[index].data)
    else
        obj_block.push(objs[index].to_string)
    end
    index = index + 1
end
blocks.push(Block.new(Array.new, obj_block))

for d in 0..dwarfs.size-2 do
    # [addr0, addr1) and [line0, line1)
    addr0 = Integer(dwarfs[d][1])
    addr1 = Integer(dwarfs[d+1][1])
    line0 = Integer(dwarfs[d][2]) - 1
    line1 = Integer(dwarfs[d+1][2]) - 1

    # add assembly to the block
    while (objs[index].is_a? Lbl) or (objs[index].index < addr0) do index = index + 1 end
    obj_block = Array.new
    while (objs[index].is_a? Lbl) or (objs[index].index < addr1) do
        if objs[index].is_a? Lbl then
            obj_block.push(objs[index].data)
        else
            obj_block.push(objs[index].to_string)
        end
        index = index + 1
    end
    
    # add code to the block
    
    blocks.push(Block.new(files[1][line0..line1], obj_block))
end

while index < objs.size do
    if objs[index].is_a? Lbl then
        obj_block.push(objs[index].data)
    else
        obj_block.push(objs[index].to_string)
    end
    index = index + 1
end
blocks.push(Block.new(Array.new, obj_block))

# Render Output

template = %q{
<html>
<head>
    <title>disassem result - <%= ARGV[1] %></title>
    <style>
        th {
            vertical-align: top;
            text-align: left;
            border: 2px solid;
        }
    </style>
</head>
<body>
<h2>disassem result - <%= ARGV[1] %></h2>
<table>
    <% blocks.each do |b| %>
        <tr>
            <th>
                <% b.src.each do |s| %> <%= s %> <br /> <% end %>
            </th>
            <th>
                <% b.asm.each do |s| %> <%= s %> <br /> <% end %>
            </th>
        </tr>
    <% end %>
</table>
</body>
</html>
}

template = ERB.new(template)
output = File.open("output.html", "w")
output.write(template.result(binding))
