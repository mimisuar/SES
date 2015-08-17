-- Store / Load Opcodes --
OP_LDA_ADDR = 0x0F
OP_LDA_BYTE = 0x5F
OP_LDA_Z = 0x7F
OP_LDZ_ADDR = 0x1F
OP_LDZ_BYTE = 0x6F
OP_LDZ_A = 0x8F
OP_STA_ADDR = 0x2F
OP_STA_ADDR = 0x3F
OP_STA_ADDR = 0x4F
-- Stack Opcodes --
OP_PUSH_BYTE = 0x0A
OP_PUSH_ADDR = 0x1A
OP_PUSH_A = 0x2A
OP_PUSH_Z = 0x3A
OP_POP_ADDR = 0x0B
OP_POP_A = 0x1B
OP_POP_Z = 0x2B
-- Branching Opcodes --
OP_RET = 0x00
OP_CALL_LABEL = 0x10
OP_JMP_ADDR = 0x20
OP_CPA_ADDR = 0x30
OP_CPA_BYTE = 0x40
OP_CPZ_ADDR = 0x50
OP_CPZ_BYTE = 0x60
OP_BEQ_ADDR = 0x70
OP_BNE_ADDR = 0x80
OP_BGT_ADDR = 0x90
OP_BLT_ADDR = 0xA0
-- Math Opcodes --
OP_ADD_BYTE = 0x0C
OP_ADD_Z = 0x2C
OP_SUB_BYTE = 0x1C
OP_SUB_Z = 0x3C
-- Flag Opcodes --
OP_EOF = 0x01
OP_EEF = 0x11
OP_EGF = 0x21
OP_ELF = 0x31
OP_DOF = 0x81
OP_DEF = 0x91
OP_DGF = 0xA1
OP_DLF = 0xB1

local codes = {}

function compileCode(data)
	for _, line in pairs(data.lines) do
		local procname = line:match("(%a+):")
		if procname then
			data.setProc(procname, data.position)
		end
		
		local inst, arg = line:match("(%a+)%s?(%g*)")
		print(line)
		
		if codes[inst] then
			codes[inst](arg, data)
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
	else
		-- ldz address --
		data.pushOpcode(OP_LDZ_ADDR).pushAddress(arg)
	end
end

function codes.sta(arg, data)
	-- sta address --
	data.pushOpcode(OP_STA_ADDR).pushAddress(arg)
end

function codes.stz(arg, data)
	-- stz address --
	data.pushOpcode(OP_STZ_ADDR).pushAddress(arg)
end

function codes.ret(arg, data)
	-- ret --
	data.pushOpcode(OP_RET)
end

function codes.call(arg, data)
	-- call label --
	data.pushOpcode(OP_CALL_LABEL).pushLabel(arg)
end

function codes.jmp(arg, data)
	-- jmp label --
	data.pushOpcode(OP_JMP_ADDR)
	if data.getProc(arg) then
		data.pushLabel(arg)
	else
		data.pushAddress(arg)
	end
end

function codes.cpa(arg, data)
	if arg:at(1) == "#" then
		-- cpa #byte --
		data.pushOpcode(OP_CPA_BYTE).pushNumber(arg)
	else
		-- cpa address --
		data.pushOpcode(OP_CPA_ADDR).pushAddress(arg)
	end
end

function codes.cpz(arg, data)
	if arg:at(1) == "#" then
		-- cpz #byte --
		data.pushOpcode(OP_CPZ_BYTE).pushNumber(arg)
	else
		data.pushOpcode(OP_CPZ_ADDR).pushAddress(arg)
	end
end

function codes.beq(arg, data)
	-- beq label --
	data.pushOpcode(OP_BEQ_ADDR)
	if data.getProc(arg) then
		data.pushLabel(arg)
	else
		data.pushAddress(arg)
	end
end

function codes.bne(arg, data)
	-- bne label --
	data.pushOpcode(OP_BNE_ADDR)
	if data.getProc(arg) then
		data.pushLabel(arg)
	else
		data.pushAddress(arg)
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
		data.pushAddress(OP_SUB_ADDR).pushAddress(arg)
	end
end

function codes.eof(arg, data)
	data.pushOpcode(OP_EOF)
end

function codes.eef(arg, data)
	data.pushOpcode(OP_EEF)
end

function codes.egf(arg, data)
	data.pushOpcode(OP_EGF)
end

function codes.elf(arg, data)
	data.pushOpcode(OP_ELF)
end

function codes.dof(arg, data)
	data.pushOpcode(OP_DOF)
end

function codes.def(arg, data)
	data.pushOpcode(OP_DEF)
end

function codes.dgf(arg, data)
	data.pushOpcode(OP_DGF)
end

function codes.def(arg, data)
	data.pushOpcode(OP_DEF)
end