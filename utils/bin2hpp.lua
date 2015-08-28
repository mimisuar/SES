function main(input, output)
	if not input or not output then
		print("bin2hpp <inputname> <outputname>")
		return
	end
	
	local codename = output:gsub("([^%a]+)", "_"):upper()
	
	local file, err = io.open(input, "rb")
	assert(file, err)
	local data = file:read("*all")
	
	local hexdata = {}
	
	for i=1, data:len() do
		table.insert(hexdata, data:byte(i))
	end
	
	file:close()
	file, err = io.open(output, "w")
	assert(file, err)
	
	file:write("#pragma once\n")
	file:write("const unsigned int " .. codename .. "_SIZE = " .. #hexdata .. ";\n")
	file:write("const char " .. codename .. "[" .. codename .. "_SIZE] = {\n")
	
	local printed = 0
	for i=1, #hexdata do
		file:write(string.format("0x%02X", hexdata[i]))
		printed = printed + 1
		
		if i < #hexdata then
			file:write(",")
		end
		
		if printed >= 5 then
			printed = 0
			file:write("\n")
		end
	end
	
	file:write("};")
	file:close()
end

main(...)