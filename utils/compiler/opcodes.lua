-- Store / Load Opcodes --
OP_LDA_ADDR 	= 0x0F
OP_LDA_BYTE 	= 0x5F
OP_LDA_Z 		= 0x7F
OP_LDA_I		= 0x02
OP_LDZ_ADDR 	= 0x1F
OP_LDZ_BYTE 	= 0x6F
OP_LDZ_A 		= 0x8F
OP_LDZ_I		= 0x12
OP_LDI_ADDR 	= 0x4F
OP_LDI_IADDR	= 0xCF
OP_STA_ADDR 	= 0x2F
OP_STA_I		= 0xE2
OP_STZ_ADDR 	= 0x3F
OP_STZ_I		= 0xF2
OP_STI_ADDR		= 0xBF

-- Stack Opcodes --
OP_PUSH_BYTE 	= 0x0A
OP_PUSH_ADDR 	= 0x1A
OP_PUSH_A 		= 0x2A
OP_PUSH_Z 		= 0x3A
OP_PUSH_I		= 0x22
OP_POP_ADDR 	= 0x0B
OP_POP_A 		= 0x1B
OP_POP_Z 		= 0x2B
OP_POP_I		= 0x32
-- Branching Opcodes --
OP_RET 			= 0x00
OP_CALL_LABEL 	= 0x10
OP_CALL_I		= 0x42
OP_JMP_ADDR 	= 0x20
OP_JMP_I		= 0x52
OP_CPA_ADDR 	= 0x30
OP_CPA_BYTE 	= 0x40
OP_CPA_I		= 0x62
OP_CPZ_ADDR 	= 0x50
OP_CPZ_BYTE 	= 0x60
OP_CPZ_I		= 0x72
OP_BEQ_ADDR 	= 0x70
OP_BEQ_I		= 0x82
OP_BNE_ADDR 	= 0x80
OP_BNE_I		= 0x92
OP_BGT_ADDR 	= 0x90
OP_BGT_I		= 0xA2
OP_BLT_ADDR 	= 0xA0
OP_BLT_I		= 0xB2
OP_POFF			= 0xB0
OP_RST			= 0xC0
-- Math Opcodes --
OP_ADD_BYTE 	= 0x0C
OP_ADD_Z 		= 0x2C		
OP_SUB_BYTE 	= 0x1C
OP_SUB_Z 		= 0x3C
OP_LSH_BYTE		= 0x4C
OP_LSH_Z		= 0xAC
OP_RSH_BYTE		= 0x5C
OP_RSH_Z		= 0xBC
OP_AND_BYTE		= 0x7C
OP_AND_Z		= 0xDC
OP_OR_BYTE		= 0x8C
OP_OR_Z			= 0xEC
OP_XOR_BYTE		= 0x9C
OP_XOR_Z		= 0xFC
OP_ADI_ADDR		= 0x03
OP_SBI_ADDR		= 0x13

-- Flag Opcodes --
OP_EOF 			= 0x01
OP_EEF 			= 0x11
OP_EGF 			= 0x21
OP_ELF 			= 0x31
OP_EGPUF		= 0x41
OP_DOF 			= 0x81
OP_DEF 			= 0x91
OP_DGF 			= 0xA1
OP_DLF 			= 0xB1
OP_DGPUF		= 0xC1
-- File Opcodes --
OP_WRA_ADDR 	= 0x9F
OP_RDA_ADDR 	= 0xAF
OP_WRA_I		= 0xC2
OP_RDA_I		= 0xD2

local codes = {}

function compileCode(data)
	for _, line in pairs(data.lines) do
		
		-- fix this line --
		for defineName, defineValue in pairs(data.defines) do
			local searchVal = "(" .. defineName .. "%g*)"
			if string.match(line, searchVal) == defineName then
			--	print(searchVal, match)
				line = line:gsub(searchVal, defineValue)
			end
		end
		
		local procname = line:match("([%a_]+):")
		if procname then
			data.setProc(procname, data.position)
		else
			
			local inst, arg = line:match("(.?[%a_]+)%s?(%g*)")
			
			if inst:at(1) == "." then
				inst, arg = line:match("(.?[%a_]+)%s?(.*)")
			end
			
			print(inst, arg)
			
			if codes[inst] then
				codes[inst](arg, data)
			end
		end
	end
	
	-- fix the code --
	for i=1, #data.toFix do
		local pos, proc = data.toFix[i].pos, data.getProc(data.toFix[i].label)
		local addr1, addr2 = proc >> 8, proc & 0xFF
		data.bytes[pos] = addr1
		data.bytes[pos + 1] = addr2
	end
end

function codes.lda(arg, data)
	if arg:at(1) == "#" then
		-- lda #byte --
		data.pushOpcode(OP_LDA_BYTE).pushNumber(arg)
	elseif arg:at(1) == "Z" then
		-- lda Z --
		data.pushOpcode(OP_LDA_Z)
	elseif arg:at(1) == "I" then
		-- lda I --
		data.pushOpcode(OP_LDA_I)
	else
		-- lda address --
		data.pushOpcode(OP_LDA_ADDR).pushAddress(arg)
	end
end

function codes.ldz(arg, data)
	if arg:at(1) == "#" then
		-- ldz #byte --
		data.pushOpcode(OP_LDZ_BYTE).pushNumber(arg)
	elseif arg:at(1) == "A" then
		-- ldz A --
		data.pushOpcode(OP_LDZ_A)
	elseif arg:at(1) == "I" then
		-- ldz I --
		data.pushOpcode(OP_LDZ_I)
	else
		-- ldz address --
		data.pushOpcode(OP_LDZ_ADDR).pushAddress(arg)
	end
end

function codes.ldi(arg, data)
	if arg:at(1):lower() == "i" then
		-- ldi Iaddress --
		data.pushOpcode(OP_LDI_IADDR).pushAddress(arg:sub(2))
	else
		-- ldi address --
		data.pushOpcode(OP_LDI_ADDR)
		if data.getProc(arg) then
			data.pushLabel(arg)
		else
			data.pushAddress(arg)
		end
	end
end

function codes.sta(arg, data)
	if arg:at(1) == "I" then
		-- sta I --
		data.pushOpcode(OP_STA_I)
	else
		-- sta address --
		data.pushOpcode(OP_STA_ADDR).pushAddress(arg)
	end
end

function codes.stz(arg, data)
	if arg:at(1) == "I" then
		-- stz I --
		data.pushOpcode(OP_STZ_I)
	else
		-- stz address --
		data.pushOpcode(OP_STZ_ADDR).pushAddress(arg)
	end
end

function codes.sti(arg, data)
	-- sti address --
	data.pushOpcode(OP_STI_ADDR).pushAddress(arg)
end

function codes.ret(arg, data)
	-- ret --
	data.pushOpcode(OP_RET)
end

function codes.call(arg, data)
	if arg:at(1) == "I" then
		-- call I --
		data.pushOpcode(OP_CALL_I)
	else
		-- call label --
		data.pushOpcode(OP_CALL_LABEL)
		if data.getProc(arg) then
			data.pushLabel(arg)
		else
			data.pushAddress(arg)
		end
	end
end

function codes.jmp(arg, data)
	if arg:at(1) == "I" then
		-- jmp I --
		data.pushOpcode(OP_JMP_I)
	else
		-- jmp label --
		data.pushOpcode(OP_JMP_ADDR)
		if data.getProc(arg) then
			data.pushLabel(arg)
		else
			data.pushAddress(arg)
		end
	end
end

function codes.cpa(arg, data)
	if arg:at(1) == "#" then
		-- cpa #byte --
		data.pushOpcode(OP_CPA_BYTE).pushNumber(arg)
	elseif arg:at(1) == "I" then
		-- cpa I --
		data.pushOpcode(OP_CPA_I)
	else
		-- cpa address --
		data.pushOpcode(OP_CPA_ADDR).pushAddress(arg)
	end
end

function codes.cpz(arg, data)
	if arg:at(1) == "#" then
		-- cpz #byte --
		data.pushOpcode(OP_CPZ_BYTE).pushNumber(arg)
	elseif arg:at(1) == "I" then
		-- cpz I --
	else
		-- cpz address --
		data.pushOpcode(OP_CPZ_ADDR).pushAddress(arg)
	end
end

function codes.beq(arg, data)
	if arg:at(1) == "I" then
		-- beq I --
		data.pushOpcode(OP_BEQ_I)
	else
		-- beq label --
		data.pushOpcode(OP_BEQ_ADDR)
		if data.getProc(arg) then
			data.pushLabel(arg)
		else
			data.pushAddress(arg)
		end
	end
end

function codes.bne(arg, data)
	if arg:at(1) == "I" then
		-- bne I --
		data.pushOpcode(OP_BNE_I)
	else
		-- bne label --
		data.pushOpcode(OP_BNE_ADDR)
		if data.getProc(arg) then
			data.pushLabel(arg)
		else
			data.pushAddress(arg)
		end
	end
end

function codes.bgt(arg, data)
	-- bgt label --
	data.pushOpcode(OP_BGT_ADDR)
	if data.getProc(arg) then
		data.pushLabel(arg)
	else
		data.pushAddress(arg)
	end
end

function codes.blt(arg, data)
	-- blt label --
	data.pushOpcode(OP_BLT_ADDR)
	if data.getProc(arg) then
		data.pushLabel(arg)
	else
		data.pushAddress(arg)
	end
end

function codes.add(arg, data)
	if arg:at(1) == "#" then
		-- add #byte --
		data.pushOpcode(OP_ADD_BYTE).pushNumber(arg)
	else
		-- add address --
		data.pushAddress(OP_ADD_ADDR).pushAddress(arg)
	end
end

function codes.sub(arg, data)
	if arg:at(1) == "#" then
		-- sub #byte --
		data.pushOpcode(OP_SUB_BYTE).pushNumber(arg)
	else
		-- sub address --
		data.pushOpcode(OP_SUB_ADDR).pushAddress(arg)
	end
end


function codes.lsh(arg, data)
	if arg:at(1) == "#" then
		-- lsh #byte --
		data.pushOpcode(OP_LSH_BYTE).pushNumber(arg)
	elseif arg:at(1) == "Z" then
		-- lsh Z --
		data.pushOpcode(OP_LSH_Z)
	end
end


function codes.rsh(arg, data)
	if arg:at(1) == "#" then
		-- rsh #byte --
		data.pushOpcode(OP_RSH_BYTE).pushNumber(arg)
	elseif arg:at(1) == "Z" then
		-- rsh Z --
		data.pushOpcode(OP_RSH_Z)
	end
end


codes["and"] = function(arg, data)
	if arg:at(1) == "#" then
		-- and #byte --
		data.pushOpcode(OP_AND_BYTE).pushNumber(arg)
	elseif arg:at(1) == "Z" then
		-- and Z --
		data.pushOpcode(OP_AND_Z)
	end
end


codes["or"] = function(arg, data)
	if arg:at(1) == "#" then
		-- or #byte --
		data.pushOpcode(OP_OR_BYTE).pushNumber(arg)
	elseif arg:at(1) == "Z" then
		-- or Z --
		data.pushOpcode(OP_OR_Z)
	end
end


function codes.xor(arg, data)
	if arg:at(1) == "#" then
		-- xor #byte --
		data.pushOpcode(OP_XOR_BYTE).pushNumber(arg)
	else
		-- xor Z --
		data.pushOpcode(OP_XOR_Z)
	end
end

function codes.adi(arg, data)
	-- adi address --
	data.pushOpcode(OP_ADI_ADDR).pushAddress(arg)
end

function codes.sbi(arg, data)
	-- sbi address --
	data.pushOpcode(OP_SBI_ADDR).pushAddress(arg)
end

function codes.eof(arg, data)
	-- eof --
	data.pushOpcode(OP_EOF)
end

function codes.eef(arg, data)
	-- eef --
	data.pushOpcode(OP_EEF)
end

function codes.egf(arg, data)
	-- egf --
	data.pushOpcode(OP_EGF)
end

function codes.elf(arg, data)
	-- elf --
	data.pushOpcode(OP_ELF)
end

function codes.egpuf(arg, data)
	-- egpuf --
	data.pushOpcode(OP_EGPUF)
end

function codes.dof(arg, data)
	-- dof --
	data.pushOpcode(OP_DOF)
end

function codes.def(arg, data)
	-- def --
	data.pushOpcode(OP_DEF)
end

function codes.dgf(arg, data)
	-- dgf --
	data.pushOpcode(OP_DGF)
end

function codes.dlf(arg, data)
	-- dlf --
	data.pushOpcode(OP_DLF)
end

function codes.dgpuf(arg, data)
	--dgpuf --
	data.pushOpcode(OP_DGPUF)
end

function codes.wra(arg, data)
	if arg:at(1) == "I" then
		-- wra I --
		data.pushOpcode(OP_WRA_I)
	else
		-- wra address --
		data.pushOpcode(OP_WRA_ADDR)
		data.pushAddress(arg)
	end
end

function codes.rda(arg, data)
	if arg:at(1) == "I" then
		-- rda I --
		data.pushOpcode(OP_RDA_I)
	else
		-- rda address --
		data.pushOpcode(OP_RDA_ADDR)
		data.pushAddress(arg)
	end
end

function codes.poff(arg, data)
	data.pushOpcode(OP_POFF)
end

function codes.rst(arg, data)
	data.pushOpcode(OP_RST)
end

-- directives --
codes[".db"] = function(arg, data)
	if io.open(arg) then
		local dbdata = assert(io.open(arg, "rb"), "No file named " .. arg .. ". Unable to include."):read("*all")
	
		local toPush = { dbdata:byte(1, dbdata:len()) }
	
		for i=1, #toPush do
			data.pushNumber(toPush[i])
		end
	else
		for number in arg:gmatch("%x+") do
			data.pushNumber(tonumber(number, 16))
		end
	end
end

codes[".proc"] = function(arg, data)
	data.setProc(arg, -1)
end

codes[".def"] = function(arg, data)
	local foundName
	for define in arg:gmatch("%g+") do
		if foundName then
			data.defines[foundName] = define
			return
		else
			foundName = define
		end
	end
end