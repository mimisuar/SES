PRGDATA_ADDRESS = 0x0500

function newData()
	-- returns an instance of the class data --
	local obj = {}
	
	obj.lines = {}
	obj.position = PRGDATA_ADDRESS
	obj.bytes = {}
	obj.toFix = {}
	obj.procs = {}
	obj.code = {}
	obj.defines = {}
	
	function obj.pushOpcode(op)
		if type(op) == "number" then
			table.insert(obj.bytes, op)
			obj.position = obj.position + 1
		else
			error("Invalid opcode.")
		end
		
		return obj -- for chaining --
	end
	
	function obj.pushNumber(number)
		-- convert the string to a number --
		local v = convertNumber(number)
		
		table.insert(obj.bytes, v)
		obj.position = obj.position + 1
		
		return obj
	end
	
	function obj.pushAddress(address)
		local n = convertNumber(address)
		local addr1, addr2 = n >> 8, n & 0xFF
		
		obj.pushNumber(addr1)
		obj.pushNumber(addr2)
		
		return obj
	end
	
	function obj.pushLabel(label)
		local n = obj.getProc(label)
		
		if not n then
			obj.showError("No such label %s.", label)
		elseif n == -1 then
			-- push negative -1 --
			table.insert(obj.bytes, -1)
			table.insert(obj.bytes, -1)
			obj.position = obj.position + 2
			table.insert(obj.toFix, {pos=#obj.bytes-1, label=label})
		else
			-- push address --
			local addr1, addr2 = n >> 8, n & 0xFF
			table.insert(obj.bytes, addr1)
			table.insert(obj.bytes, addr2)
			obj.position = obj.position + 2
		end
		
		return obj
	end
	
	function obj.setProc(procname, v)
		obj.procs[procname] = v
	end
	
	function obj.getProc(procname)
		return obj.procs[procname]
	end
	
	function obj.showError(errmsg, ...)
		error("Error on line " .. "" .. ": " .. string.format(errmsg, ...) .. ".")
	end
	
	function obj.addLine(line, ogline)
		if line ~= "" then
			table.insert(obj.lines, line)
			table.insert(obj.code, ogline)
		end
	end
	
	return obj
end

function cleanFile(fname)
	local input = assert(io.open(fname), "Unable to open file " .. fname .. ".")
	local data = newData()
	
	local linecount = 0
	
	local lines = {}
	
	for line in input:lines() do
		linecount = linecount + 1
		
		local ogline = linecount
		
		while line:at(1) == " " or line:at(1) == "\t" do
			line = line:sub(2)
		end
		
		local commentPosition = line:find(";")
		if commentPosition then
			line = line:sub(1, commentPosition - 1)
		end
		
		while line:at(line:len()) == " " or line:at(line:len()) == "\t" do
			line = line:sub(1, line:len() - 1)
		end
		
		local procname = line:match("([%a_]+):")
		if procname then
			data.setProc(procname, -1)
		end
		
		if line:at(1) == "@" then
			local tmpdata = cleanFile(line:sub(2))
			for i = 1, #tmpdata.lines do
				data.addLine(tmpdata.lines[i], tmpdata.code[i])
				for key, value in pairs(tmpdata.procs) do
				--	assert(not data.procs[key], "Conflicting procedure name found. Please fix this.")
					data.procs[key] = tmpdata.procs[key]
				end
			end
		else
			data.addLine(line, ogline)
		end
	end
	
	input:close()
	
	return data
end

function string.at(str, pos)
	return str:sub(pos, pos)
end

function convertNumber(n)
	local v
	
	if type(n) == "number" then
		return n
	end
	
	if n:at(1) == "#" then
		n = n:sub(2)
	end
	
	if n:at(1) == "$" then
		v = tonumber(n:sub(2), 16)
	elseif n:at(1) == "%" then
		v = tonumber(n:sub(2), 2)
	else
		v = tonumber(n:sub(1))
	end
	
	if not v then
		error("Invalid number: " .. tostring(v))
	end
	
	return v
end

function writeToFile(data, fname)
	local output = assert(io.open(fname, "w"), "Unable to open " .. fname .. " for writing")
	
	for i=1, #data.bytes do
		output:write(string.char(data.bytes[i]))
	end
	
	output:close()
end