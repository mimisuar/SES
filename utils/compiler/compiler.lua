assert(_VERSION == "Lua 5.3", "This a Lua 5.3 script, you are using " .. _VERSION .. ".")

require("util")
require("opcodes")

function main(input, output)
	if not input or not output then
		showUsage()
	end
	
	-- clean up the file --
	local data = cleanFile(input)
	
	-- compile the code --
	compileCode(data)
	
	-- finally write output file --
	writeToFile(data, output)
end

function showUsage()
	print("compiler.lua <source-file> <outputfile>")
	os.exit(0)
end

if not _MAIN then
	main(...)
else
	compiler = main
end