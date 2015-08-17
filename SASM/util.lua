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
	
	function obj.pushOpcode(op)
		if op > -1 then
			table.insert(obj.bytes, op)
			obj.position = obj.position + 1
		else
			
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
		local addr1, addr2 = address >> 8, address & 0xFF
		table.insert(obj.bytes, addr1)
		table.insert(obj.bytes, addr2)
		
		obj.position = obj.position + 1
		
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
		
		local procname = line:match("(%a+):")
		if procname then
			data.setProc(procname, -1)
		end
		
		data.addLine(line, ogline)
	end
	
	input:close()
	
	return data
end

function string.at(str, pos)
	return str:sub(pos, pos)
end

function convertNumber(n)
	local v
	
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
	local output = io.open(fname, "w")
	
	for i=1, #data.bytes do
		output:write(string.char(data.bytes[i]))
	end
	
	output:close()
end