#include "CPU.hpp"

#include <iostream>
#include <string>


namespace SES
{
	CPU::CPU()
	{
		m_accumulator = 0;
		m_zindex = 0;
		m_index = 0;
		m_datastack = 0;
		m_statusflag = 0;
		m_pc = PRGROM_ADDRESS;
		m_callsp = 0;
		m_ememfile = "";

		// Status Flag //
		// bit 0 - overflow / underflow. If during math the value in the accumulator goes over or under 255 or 0. //

		for (word addr = 0; addr < MEMORY_SIZE; addr++)
		{
			m_ram[addr] = 0;
		}

		for (word addr = 0; addr < EXTERNAL_MEMORY_SIZE; addr++)
		{
			m_emem[addr] = 0;
		}
	}

	CPU::~CPU()
	{

	}

	void CPU::Reset()
	{
		// clear everything except for program data //
		this->SaveExternalMem();
		m_pc = PRGROM_ADDRESS;

		for (word addr = 0; addr < PRGROM_ADDRESS; addr++)
		{
			m_ram[addr] = 0;
		}


	}

	void CPU::WriteByte(word address, byte value)
	{
		if (address >= 0 && address < MEMORY_SIZE)
		{
			m_ram[address] = value;
			if (address == GPU_PORT)
			{
				FlagEnableBit(FLAG_GPU);
			}
		}
	}

	byte CPU::ReadByte(word address)
	{
		if (address >= 0 && address < MEMORY_SIZE)
		{
			return m_ram[address];
		}
		return 0;
	}

	void CPU::PushStack(byte value)
	{
		if (m_datastack <= 15)
		{
			this->WriteByte(STACK_ADDRESS + m_datastack, value);
			m_datastack++;
		}
	}

	byte CPU::PopStack()
	{
		if (m_datastack > 0)
		{
			m_datastack--;
			return this->ReadByte(STACK_ADDRESS + m_datastack);
		}
		return 0;
	}

	void CPU::InitExternalMem(const char* fname)
	{
		std::string str1, str2;
		str1 = "emem/";
		str2 = fname;
		m_ememfile_str = (str1 + str2);
		m_ememfile = m_ememfile_str.c_str();
		

		FILE* externalmem = fopen(m_ememfile, "r");
		if (!externalmem)
		{
			std::cout << "Unable to read memory." << std::endl;
		}
		else
		{
			fread(m_emem, sizeof(byte), EXTERNAL_MEMORY_SIZE, externalmem);
			fclose(externalmem);
		}
	}

	void CPU::SaveExternalMem()
	{
		FILE* externalmem = fopen(m_ememfile, "w");
		if (!externalmem)
		{
			std::cout << "Unable to write memory." << std::endl;
		}
		else
		{
			fwrite(m_emem, sizeof(byte), EXTERNAL_MEMORY_SIZE, externalmem);
			fclose(externalmem);
		}
	}

	void CPU::Run()
	{
		byte opcode = this->ReadByte(m_pc);
		word addr = 0;
		switch (opcode)
		{
		case OP_LDA_ADDR:
			// Load the value at a memory address into the accumulator //
			addr = (this->ReadByte(++m_pc) << 8) | this->ReadByte(++m_pc);
			m_accumulator = this->ReadByte(addr);
			m_pc++;
			break;

		case OP_LDA_BYTE:
			// Load a byte into the accumulator //
			m_accumulator = this->ReadByte(++m_pc);
			m_pc++;
			break;

		case OP_LDA_Z:
			// Load the value in the z-index into the accumulator //
			m_accumulator = m_zindex;
			m_pc++;
			break;

		case OP_LDA_I:
			// Load the value at address I into the accumulator //
			m_accumulator = this->ReadByte(m_index);
			m_pc++;
			break;

		case OP_LDZ_ADDR:
			// Load a value from a memory address into the z-index //
			addr = (this->ReadByte(++m_pc) << 8) | this->ReadByte(++m_pc);
			m_zindex = this->ReadByte(addr);
			m_pc++;
			break;

		case OP_LDZ_BYTE:
			// Load byte into the z-index //
			m_zindex = this->ReadByte(++m_pc);
			m_pc++;
			break;

		case OP_LDZ_A:
			// Load the value in the accumulator into the z-index //
			m_zindex = m_accumulator;
			m_pc++;
			break;

		case OP_LDZ_I:
			// Load the value at address I into the Z-index //
			m_zindex = this->ReadByte(m_index);
			break;

		case OP_LDI_ADDR:
			// Load the address into I //
			m_index = (this->ReadByte(++m_pc) << 8) | this->ReadByte(++m_pc);
			m_pc++;
			break;

		case OP_LDI_IADDR:
			// Load at the value at address addr into I //
			addr = (this->ReadByte(++m_pc) << 8) | this->ReadByte(++m_pc);
			m_index = (this->ReadByte(addr) << 8) | this->ReadByte(addr + 1);
			m_pc++;
			break;

		case OP_STA_ADDR:
			// Store the value in the accumulator into a memory address //
			addr = (this->ReadByte(++m_pc) << 8) | this->ReadByte(++m_pc);
			this->WriteByte(addr, m_accumulator);
			m_pc++;
			break;

		case OP_STZ_ADDR:
			// Store the value in the Z-index into a memory address //
			addr = (this->ReadByte(++m_pc) << 8) | this->ReadByte(++m_pc);
			this->WriteByte(addr, m_zindex);
			m_pc++;
			break;

		case OP_STI_ADDR:
			// Store the value OF the index at address addr //
			addr = (this->ReadByte(++m_pc) << 8) | this->ReadByte(++m_pc);
			this->WriteByte(addr, m_index >> 8);
			this->WriteByte(addr + 1, m_index & 0xFF);
			m_pc++;
			break;

		case OP_PUSH_BYTE:
			// Push an immediate byte onto the stack //
			this->PushStack(this->ReadByte(++m_pc));
			m_pc++;
			break;

		case OP_PUSH_ADDR:
			// Push a value from a memory address onto the stack //
			addr = (this->ReadByte(++m_pc) << 8) | this->ReadByte(++m_pc);
			this->PushStack(this->ReadByte(addr));
			m_pc++;
			break;

		case OP_PUSH_A:
			// Push the accumulator onto the stack //
			this->PushStack(m_accumulator);
			m_pc++;
			break;

		case OP_PUSH_Z:
			// Push the Z-index onto the stack //
			this->PushStack(m_zindex);
			m_pc++;
			break;

		case OP_PUSH_I:
			// Push the value at address I onto the stack //
			this->PushStack(this->ReadByte(m_index));
			m_pc++;
			break;

		case OP_POP_ADDR:
			// Pop the top of the stack into a memory address //
			addr = (this->ReadByte(++m_pc) << 8) | this->ReadByte(++m_pc);
			this->WriteByte(addr, this->PopStack());
			m_pc++;
			break;

		case OP_POP_A:
			// Pop the top of the stack into the accumulator //
			m_accumulator = this->PopStack();
			m_pc++;
			break;

		case OP_POP_Z:
			// Pop the top of the stack into the Z-index //
			m_zindex = this->PopStack();
			m_pc++;
			break;

		case OP_POP_I:
			// Pop the top of the stack into address I //
			this->WriteByte(m_index, this->PopStack());
			m_pc++;
			break;

		case OP_CALL:
			// Call a sub-routine //
			addr = (this->ReadByte(++m_pc) << 8) | this->ReadByte(++m_pc);
			if (m_callsp < 16)
			{
				m_callstack[m_callsp++] = m_pc;
				m_pc = addr;
			}
			else
			{
				printf("Stack overflow error!");
				FlagEnableBit(FLAG_POFF);
			}
			break;

		case OP_CALL_I:
			// Call a sub-routine at address I //
			if (m_callsp < 16)
			{
				m_callstack[m_callsp++] = m_pc;
				m_pc = m_index;
			}
			else
			{
				printf("Stack overflow error!");
				FlagEnableBit(FLAG_POFF);
			}
			break;

		case OP_RET:
			// Return from a call //
			if (m_callsp > 0)
			{
				m_pc = m_callstack[--m_callsp];
				m_pc++;
			}
			break;

		case OP_JMP:
			// Jump to a different location in the code //
			addr = (this->ReadByte(++m_pc) << 8) | this->ReadByte(++m_pc);
			m_pc = addr;
			break;

		case OP_JMP_I:
			// Jump to a different location in the code //
			m_pc = m_index;
			break;

		case OP_CPA_ADDR:
			// Compare the value in the accumulator with 
			addr = (this->ReadByte(++m_pc) << 8) | this->ReadByte(++m_pc);
			FlagDisableBit(FLAG_EQUAL | FLAG_GREATER | FLAG_LESS);

			if (m_accumulator == this->ReadByte(addr))
			{
				FlagEnableBit(FLAG_EQUAL);
			}
			else if (m_accumulator > this->ReadByte(addr))
			{
				FlagEnableBit(FLAG_GREATER);
			}
			else if (m_accumulator < this->ReadByte(addr))
			{
				FlagEnableBit(FLAG_LESS);
			}
			m_pc++;
			break;

		case OP_CPA_BYTE:
			FlagDisableBit(FLAG_EQUAL | FLAG_GREATER | FLAG_LESS);

			m_pc++;
			if (m_accumulator == this->ReadByte(m_pc))
			{
				FlagEnableBit(FLAG_EQUAL);
			}
			else if (m_accumulator > this->ReadByte(m_pc))
			{
				FlagEnableBit(FLAG_GREATER);
			}
			else if (m_accumulator < this->ReadByte(m_pc))
			{
				FlagEnableBit(FLAG_LESS);
			}
			m_pc++;
			break;

		case OP_CPA_I:
			FlagDisableBit(FLAG_EQUAL | FLAG_GREATER | FLAG_LESS);

			if (m_accumulator == this->ReadByte(m_index))
			{
				FlagEnableBit(FLAG_EQUAL);
			}
			else if (m_accumulator > this->ReadByte(m_index))
			{
				FlagEnableBit(FLAG_GREATER);
			}
			else if (m_accumulator < this->ReadByte(m_index))
			{
				FlagDisableBit(FLAG_LESS);
			}
			m_pc++;
			break;

		case OP_CPZ_ADDR:
			addr = (this->ReadByte(++m_pc) << 8) | this->ReadByte(++m_pc);
			FlagDisableBit(FLAG_EQUAL | FLAG_GREATER | FLAG_LESS);

			if (m_zindex == this->ReadByte(addr))
			{
				FlagEnableBit(FLAG_EQUAL);
			}
			else if (m_zindex > this->ReadByte(addr))
			{
				FlagEnableBit(FLAG_GREATER);
			}
			else if (m_zindex < this->ReadByte(addr))
			{
				FlagEnableBit(FLAG_LESS);
			}
			m_pc++;
			break;

		case OP_CPZ_BYTE:
			FlagDisableBit(FLAG_EQUAL | FLAG_GREATER | FLAG_LESS);

			m_pc++;
			if (m_zindex == this->ReadByte(m_pc))
			{
				FlagEnableBit(FLAG_EQUAL);
			}
			else if (m_zindex > this->ReadByte(m_pc))
			{
				FlagEnableBit(FLAG_GREATER);
			}
			else if (m_zindex < this->ReadByte(m_pc))
			{
				FlagEnableBit(FLAG_LESS);
			}
			
			m_pc++;
			break;

		case OP_CPZ_I:
			FlagDisableBit(FLAG_EQUAL | FLAG_GREATER | FLAG_LESS);

			m_pc++;
			if (m_zindex == this->ReadByte(m_index))
			{
				FlagEnableBit(FLAG_EQUAL);
			}
			else if (m_zindex > this->ReadByte(m_index))
			{
				FlagEnableBit(FLAG_GREATER);
			}
			else if (m_zindex < this->ReadByte(m_index))
			{
				FlagEnableBit(FLAG_LESS);
			}
			m_pc++;
			break;

		case OP_BEQ_ADDR:
			addr = (this->ReadByte(++m_pc) << 8) | this->ReadByte(++m_pc);
			if (FlagIsEnabled(FLAG_EQUAL))
			{
				m_pc = addr;
			}
			else
			{
				m_pc++;
			}
			break;

		case OP_BEQ_I:
			if (FlagIsEnabled(FLAG_EQUAL))
			{
				m_pc = m_index;
			}
			else
			{
				m_pc++;
			}
			break;

		case OP_BNE_ADDR:
			addr = (this->ReadByte(++m_pc) << 8) | this->ReadByte(++m_pc);
			if (!FlagIsEnabled(FLAG_EQUAL))
			{
				m_pc = addr;
			}
			else
			{
				m_pc++;
			}
			break;

		case OP_BNE_I:
			if (!FlagIsEnabled(FLAG_EQUAL))
			{
				m_pc = m_index;
			}
			else
			{
				m_pc++;
			}
			break;

		case OP_BGT_ADDR:
			addr = (this->ReadByte(++m_pc) << 8) | this->ReadByte(++m_pc);
			if (FlagIsEnabled(FLAG_GREATER))
			{
				m_pc = addr;
			}
			else
			{
				m_pc++;
			}
			break;

		case OP_BGT_I:
			if (FlagIsEnabled(FLAG_GREATER))
			{
				m_pc = m_index;
			}
			else
			{
				m_pc++;
			}
			break;

		case OP_BLT_ADDR:
			addr = (this->ReadByte(++m_pc) << 8) | this->ReadByte(++m_pc);
			if (FlagIsEnabled(FLAG_LESS))
			{
				m_pc = addr;
			}
			else
			{
				m_pc++;
			}
			break;

		case OP_BLT_I:
			if (FlagIsEnabled(FLAG_LESS))
			{
				m_pc = m_index;
			}
			else
			{
				m_pc++;
			}
			break;

		case OP_POFF:
			// Exit from running and power off the system //
			FlagEnableBit(FLAG_POFF);
			break;

		case OP_RST:
			// Reset the system //
			FlagEnableBit(FLAG_RESET);
			break;

		case OP_ADD_BYTE:
			// Add immediate byte to the accumulator. If the value is greater than 255, set overflow bit to 1 //
			FlagDisableBit(FLAG_OVERFLOW);

			if (m_accumulator + this->ReadByte(++m_pc) > 0xFF)
			{
				FlagEnableBit(FLAG_OVERFLOW);
			}

			m_accumulator += this->ReadByte(m_pc++);
			break;

		case OP_ADD_Z:
			// Add Z to the accumulator. if the value is greater than 255, set the overflow bit to 1 //
			FlagDisableBit(FLAG_OVERFLOW);

			if (m_accumulator + m_zindex > 0xFF)
			{
				FlagEnableBit(FLAG_OVERFLOW);
			}

			m_accumulator += m_zindex;
			m_pc++;
			break;

		case OP_SUB_BYTE:
			// Subtract immediate byte from the accumulator. If the value is less than 0, set the underflow bit (it's just the overflow bit) to 1 //
			FlagDisableBit(FLAG_OVERFLOW);

			if (m_accumulator - this->ReadByte(++m_pc) < 0)
			{
				FlagEnableBit(FLAG_OVERFLOW);
			}

			m_accumulator -= this->ReadByte(m_pc++);
			break;

		case OP_SUB_Z:
			// Subtract Z from the accumulator. If the value is less than 0, set the underflow bit to 1 //
			FlagDisableBit(FLAG_OVERFLOW);

			if (m_accumulator - this->ReadByte(++m_pc) < 0)
			{
				FlagEnableBit(FLAG_OVERFLOW);
			}

			m_accumulator -= this->ReadByte(m_pc++);
			break;

		case OP_LSH_BYTE:
			// Left-shift by an immediate byte. //
			m_accumulator <<= this->ReadByte(++m_pc);
			m_pc++;
			break;

		case OP_LSH_Z:
			// Left-shift by Z. //
			m_accumulator <<= m_zindex;
			m_pc++;
			break;

		case OP_RSH_BYTE:
			// Right-shift by an immediate byte. //
			m_accumulator >>= this->ReadByte(++m_pc);
			m_pc++;
			break;

		case OP_RSH_Z:
			// Right-shift by Z. //
			m_accumulator >>= m_zindex;
			m_pc++;
			break;

		case OP_AND_BYTE:
			// Bitwise-and A and an immediate byte //
			m_accumulator &= this->ReadByte(++m_pc);
			m_pc++;
			break;

		case OP_AND_Z:
			// Bitwise-and A and Z //
			m_accumulator &= m_zindex;
			m_pc++;
			break;

		case OP_OR_BYTE:
			// Bitwise-or A and an immediate byte //
			m_accumulator |= this->ReadByte(++m_pc);
			m_pc++;
			break;

		case OP_OR_Z:
			// Bitwise-or A and Z //
			m_accumulator |= m_zindex;
			m_pc++;
			break;

		case OP_XOR_BYTE:
			// Bitwise-xor A and an immediate byte //
			m_accumulator ^= this->ReadByte(++m_pc);
			m_pc++;
			break;

		case OP_XOR_Z:
			// Bitwise-xor A and Z //
			m_accumulator ^= m_zindex;
			m_pc++;
			break;

		case OP_ADI_ADDR:
			// Add an address to index //
			FlagDisableBit(FLAG_OVERFLOW);
			addr = (this->ReadByte(++m_pc) << 8) | this->ReadByte(++m_pc);

			if (m_index + addr > MEMORY_SIZE)
			{
				FlagEnableBit(FLAG_OVERFLOW);
			}

			m_index += addr;
			m_pc++;
			break;

		case OP_SBI_ADDR:
			// Add an address to index //
			FlagDisableBit(FLAG_OVERFLOW);
			addr = (this->ReadByte(++m_pc) << 8) | this->ReadByte(++m_pc);

			if (m_index - this->ReadByte(addr) < MEMORY_SIZE)
			{
				FlagEnableBit(FLAG_OVERFLOW);
			}

			m_index -= addr;
			m_pc++;
			break;

		case OP_EOF:
			// Enable overflow bit //
			FlagEnableBit(FLAG_OVERFLOW);
			m_pc++;
			break;

		case OP_EEF:
			// Enable equal bit //
			FlagEnableBit(FLAG_EQUAL);
			m_pc++;
			break;

		case OP_EGF:
			// Enable greater bit //
			FlagEnableBit(FLAG_GREATER);
			m_pc++;
			break;

		case OP_ELF:
			// Enable less bit //
			FlagEnableBit(FLAG_LESS);
			m_pc++;
			break;

		case OP_EGPUF:
			// Enable GPU write bit //
			FlagEnableBit(FLAG_GPU);
			m_pc++;
			break;

		case OP_DOF:
			// Disable overflow bit //
			FlagDisableBit(FLAG_OVERFLOW);
			m_pc++;
			break;

		case OP_DEF:
			// Disable equal bit //
			FlagDisableBit(FLAG_EQUAL);
			m_pc++;
			break;

		case OP_DGF:
			// Disable greater bit //
			FlagDisableBit(FLAG_GREATER);
			m_pc++;
			break;

		case OP_DLF:
			// Disable less bit //
			FlagDisableBit(FLAG_LESS);
			m_pc++;
			break;

		case OP_DGPUF:
			// Disable GPU bit //
			FlagDisableBit(FLAG_GPU);
			m_pc++;
			break;

		case OP_WRA_ADDR:
			// Write to external data //
			addr = (this->ReadByte(++m_pc) << 8) | this->ReadByte(++m_pc);
			if (addr < EXTERNAL_MEMORY_SIZE)
			{
				m_emem[addr] = m_accumulator;
			}
			else
			{
				std::cout << "External memory only has " << EXTERNAL_MEMORY_SIZE << " bytes." << std::endl;
			}
			m_pc++;
			break;

		case OP_RDA_ADDR:
			// Read from external data //
			addr = (this->ReadByte(++m_pc) << 8) | this->ReadByte(++m_pc);
			if (addr < EXTERNAL_MEMORY_SIZE)
			{
				m_accumulator = m_emem[addr];
			}
			else
			{
				std::cout << "External memory only has " << EXTERNAL_MEMORY_SIZE << " bytes." << std::endl;
			}
			m_pc++;
			break;

		case OP_WRA_I:
			// Write to external data //
			if (m_index < EXTERNAL_MEMORY_SIZE)
			{
				m_emem[m_index] = m_accumulator;
			}
			else
			{
				std::cout << "External memory only has " << EXTERNAL_MEMORY_SIZE << " bytes." << std::endl;
			}
			m_pc++;
			break;

		case OP_RDA_I:
			// Read from external data //
			if (m_index < EXTERNAL_MEMORY_SIZE)
			{
				m_accumulator = m_emem[m_index];
			}
			else
			{
				std::cout << "External memory only has " << EXTERNAL_MEMORY_SIZE << " bytes." << std::endl;
			}
			m_pc++;
			break;

		default:
			std::cout << "Unknown opcode " << (int)opcode << "." << std::endl;
			m_pc++;
			break;
		}
	}

	bool CPU::LoadRom(const char* fname)
	{
		this->InitExternalMem(fname);

		FILE* file;
		if (fopen_s(&file, fname, "rb") != 0)
		{
			printf("Unable to open romfile %s.\n", fname);
			return false;
		}

		fseek(file, 0, SEEK_END);
		int filesize = ftell(file);
		rewind(file);
		
		char romcode;
		for (int pos = 0; pos < filesize; pos++)
		{
			romcode = fgetc(file);
			m_ram[PRGROM_ADDRESS + pos] = romcode;
		}

		fclose(file);

		return true;
	}

	void CPU::FlagEnableBit(byte mask)
	{
		m_statusflag |= mask;
	}

	void CPU::FlagDisableBit(byte mask)
	{
		m_statusflag &= ~(mask);
	}

	void CPU::FlagToggleBit(byte mask)
	{
		m_statusflag ^= mask;
	}

	bool CPU::FlagIsEnabled(byte mask)
	{
		return ((m_statusflag & mask) == mask);
	}
}