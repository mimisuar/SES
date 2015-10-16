error("THIS IS THE OLD COMPILER. DON'T USE THIS!")
assert(_VERSION == "Lua 5.3", "This a Lua 5.3 script, you are using " .. _VERSION .. ".")

local args = {...}

local input, output = ...

if not input or not output then
	print("lua compiler.lua <source-file> <output-file>")
	return
end

local file = io.open(input)

if not file then
	error(input .. " doesn't exist.")
end

function at(str, pos)
	return str:sub(pos, pos)
end



-- go through and clean up the file --
local data = {}
local procs = {}
local bytes = {}
local position = 0x250 -- starting point of the rom data --
local linedata = {} -- this is for error handling --
local pos = 0
local toFix = {}

function formatError(pos)
	return "Line " .. tostring(linedata[pos][1]) .. ": " .. tostring(linedata[pos][2]) .. "\n"
	-- return "ERROR: "
end

function convertNumber(numberstring)
	local ret
	if at(numberstring, 1) == "$" then
		-- hex number --
		ret = tonumber(numberstring:sub(2), 16)
	elseif at(numberstring, 1) == "%" then
		-- binary number --
		ret = tonumber(numberstring:sub(2), 2)
	else
		-- decimal number --
		ret = tonumber(numberstring)
	end
	
	assert(ret, formatError(pos) .. "Invalid number.")
	return ret
end

function convertToAddress(nstr)
	local n = convertNumber(nstr)
	
	return n >> 8, n & 0xFF
end

local linecount = 0
for line in file:lines() do
	linecount = linecount + 1
	
	local originalline = line
	
	while at(line, 1) == ' ' or at(line, 1) == '\t' do
		line = line:sub(2, line:len())
	end
	
	if line:find(";") then
		local pos = line:find(";")
		line = line:sub(1, pos - 1)
	end
	
	while at(line, line:len()) == ' ' or at(line, line:len()) == '\t' do
		line = line:sub(1, line:len() - 1)
	end
	
	table.insert(data, line)
	table.insert(linedata, {linecount, originalline})
	
	if line:match("(%a+):") then
		procs[line:match("(%a+):")] = -1 -- setup the name here --
	end
end

for _, line in pairs(data) do
	pos = _
	local m = line:match("(%a+):")
	if m then
		assert(procs[m] == -1, formatError(pos) .. " Attempting to redefine procedure.")
		
		procs[m] = position
	end
	
	-- actually handle code stuff now --
	
	local inst, arg = line:match("(%a+)%s*(%g+)")
	print(inst, arg)
	if inst == "lda" then
		-- byte or address --
		if at(arg, 1) == "#" then
			-- lda #byte --
			table.insert(bytes, 0x5F)
			table.insert(bytes, convertNumber(arg:sub(2)) & 0xFF)
			position = position + 2
		elseif at(arg, 1) == "Z" then
			-- lda Z --
			table.insert(bytes, 0x7F)
			position = position + 1
		else
			-- lda address --
			local addr1, addr2 = convertToAddress(arg)
			table.insert(bytes, 0x0F)
			table.insert(bytes, addr1)
			table.insert(bytes, addr2)
			position = position + 3
		end
	elseif inst == "ldz" then
		if at(arg, 1) == "#" then
			-- ldz #byte --
			table.insert(bytes, 0x6F)
			table.insert(bytes, convertNumber(arg:sub(2)) & 0xFF)
			position = position + 2
		elseif at(arg, 1) == "A" then
			-- ldz A --
			table.insert(bytes, 0x8F)
			position = position + 1
		else
			-- ldz address --
			local addr1, addr2 = convertToAddress(arg)
			table.insert(bytes, 0x1F)
			table.insert(bytes, addr1)
			table.insert(bytes, addr2)
			position = position + 3
		end
	elseif inst == "sta" then
		-- sta address --
		local addr1, addr2 = convertToAddress(arg)
		table.insert(bytes, 0x2F)
		table.insert(bytes, addr1)
		table.insert(bytes, addr2)
		position = position + 3
	elseif inst == "stz" then
		-- stz address --
		local addr1, addr2 = convertToAddress(arg)
		table.insert(bytes, 0x3F)
		table.insert(bytes, addr1)
		table.insert(bytes, addr2)
		position = position + 3
	elseif inst == "stf" then
		-- stf address --
		local addr1, addr2 = convertToAddress(arg)
		table.insert(bytes, 0x4F)
		table.insert(bytes, addr1)
		table.insert(bytes, addr2)
		position = position + 3
	elseif inst == "add" then
		if at(arg, 1) == "#" then
			-- add #byte --
			table.insert(bytes, 0x0C)
			table.insert(bytes, convertToNumber(arg:sub(2)) & 0xFF)
			position = position + 2
		elseif at(arg, 1) == "Z" then
			-- add Z --
			table.insert(bytes, 0x2C)
			position = position + 1
		else
			-- add address --
			local addr1, addr2 = convertToAddress(arg)
			table.insert(bytes, 0x1C)
			table.insert(bytes, addr1)
			table.insert(bytes, addr2)
			position = position + 3
		end
	elseif inst == "call" then
		-- call label --
		assert(procs[arg], formatError(pos) .. "Attempting to call invalid procedure " .. arg .. ".")
		table.insert(bytes, 0x10)
		if procs[arg] == -1 then
			table.insert(bytes, -1)
			table.insert(bytes, -1)
			table.insert(toFix, {pos=#bytes-1, proc=arg})
		else
			table.insert(bytes, procs[arg] >> 8)
			table.insert(bytes, procs[arg] & 0xFF)
		end
		position = position + 3
	elseif inst == "jmp" then
		table.insert(bytes, 0x20)
		if procs[arg] then
			-- jmp label --
			if procs[arg] == -1 then
				table.insert(bytes, -1)
				table.insert(bytes, -1)
				table.insert(toFix, {pos=#bytes-1, proc=arg})
			else
				table.insert(bytes, procs[arg] >> 8)
				table.insert(bytes, procs[arg] & 0xFF)
			end
		else
			-- jump addr --
			local addr1, addr2 = convertToAddress(arg)
			table.insert(bytes, addr1)
			table.insert(bytes, addr2)
		end
		
		position = position + 3
	elseif inst == "ret" then
		-- ret --
		table.insert(bytes, 0x00)
		position = position + 1
	elseif inst == "sub" then
		if at(arg, 1) == "#" then
			-- sub #byte --
			table.insert(bytes, 0x1C)
			table.insert(bytes, convertNumber(arg:sub(2)))
			position = position + 2
		else
			-- sub Z --
			table.insert(bytes, 0x3C)
			position = position + 1
		end
	elseif inst == "push" then
		if at(arg, 1) == "#" then
			-- push #byte --
			table.insert(bytes, 0x0A)
			table.insert(bytes, convertNumber(arg:sub(2))
			position = position + 2
		elseif at(arg, 1) == "A" then
			-- push A --
			table.insert(bytes, 0x2A)
			position = position + 1
		elseif at(arg, 1) == "Z" then
			-- push Z --
			table.insert(bytes, 0x3A)
			position = position + 1
		elseif at(arg, 1) == "F" then
			-- push F --
			table.insert(bytes, 0x4A)
			position = position + 1
		else
			-- push address --
			local addr1, addr2 = convertToAddress(arg)
			table.insert(bytes, 0x1A)
			table.insert(bytes, addr1)
			table.insert(bytes, addr2)
			position = position + 3
		end
	elseif inst == "pop" then
		if at(arg, 1) == "A" then
			-- pop A --
			table.insert(bytes, 0x1B)
			position = position + 1
		elseif at(arg, 1) == "Z" then
			-- pop Z --
			table.insert(bytes, 0x2B)
			position = position + 1
		else
			-- pop address --
			local addr1, addr2 = convertToAddress(arg)
			table.insert(bytes, 0x0B)
			table.insert(bytes, addr1)
			table.insert(bytes, addr2)
			position = position + 3
		end
	end
end

-- fix em! --
for i=1, #toFix do
	local position, procAddr = toFix[i].pos, procs[toFix[1].proc]
	bytes[position] = procAddr >> 8
	bytes[position + 1] = procAddr & 0xFF
end

-- write it all to a file --
file = io.open(output, "w")
for i=1, #bytes do
	file:write(string.char(bytes[i]))
end
file:close()