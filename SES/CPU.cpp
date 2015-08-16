#include "CPU.hpp"

#include <iostream>

namespace SES
{
	CPU::CPU()
	{
		m_accumulator = 0;
		m_zindex = 0;
		m_datastack = 0;
		m_statusflag = 0;
		m_pc = PRGROM_ADDRESS;
		m_callsp = 0;

		// Status Flag //
		// bit 0 - overflow / underflow. If during math the value in the accumulator goes over or under 255 or 0. //

		for (word addr = 0; addr < MEMORY_SIZE; addr++)
		{
			m_ram[addr] = 0;
		}
	}

	CPU::~CPU()
	{

	}

	void CPU::Reset()
	{
		// clear everything except for program data //
		m_pc = PRGROM_ADDRESS;

		for (word addr = 0; addr < PRGROM_ADDRESS; addr++)
		{
			m_ram[addr] = 0;
		}

		
	}

	void CPU::WriteByte(word address, byte value)
	{
		m_ram[address] = value;
	}

	byte CPU::ReadByte(word address)
	{
		return m_ram[address];
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
			m_accumulator = m_zindex;
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

		case OP_STF_ADDR:
			// Store the value of the status flag into a memory address //
			addr = (this->ReadByte(++m_pc) << 8) | this->ReadByte(++m_pc);
			this->WriteByte(addr, m_statusflag);
			break;

		case OP_PUSH_BYTE:
			// Push an immediate byte onto the stack //
			if (m_datastack <= 15)
			{
				this->WriteByte(STACK_ADDRESS + m_datastack, this->ReadByte(++m_pc));
				m_datastack++;
			}
			else
			{
				m_pc++;
			}
			m_pc++;
			break;

		case OP_PUSH_ADDR:
			// Push a value from a memory address onto the stack //
			if (m_datastack <= 15)
			{
				addr = (this->ReadByte(++m_pc) << 8) | this->ReadByte(++m_pc);
				this->WriteByte(STACK_ADDRESS + m_datastack, this->ReadByte(addr));
				m_datastack++;
			}
			else
			{
				m_pc += 2;
			}
			m_pc++;
			break;

		case OP_PUSH_A:
			// Push the accumulator onto the stack //
			if (m_datastack <= 15)
			{
				this->WriteByte(STACK_ADDRESS + m_datastack, m_accumulator);
				m_datastack++;
			}
			m_pc++;
			break;

		case OP_PUSH_Z:
			// Push the Z-index onto the stack //
			if (m_datastack <= 15)
			{
				this->WriteByte(STACK_ADDRESS + m_datastack, m_zindex);
				m_datastack++;
			}
			break;

		case OP_PUSH_F:
			// Push the status flag onto the stack //
			if (m_datastack <= 15)
			{
				this->WriteByte(STACK_ADDRESS + m_datastack, m_statusflag);
				m_datastack++;
			}
			break;

		case OP_POP_ADDR:
			// Pop the top of the stack into a memory address //
			if (m_datastack > 0)
			{
				addr = (this->ReadByte(++m_pc) << 8) | this->ReadByte(++m_pc);
				m_datastack--;
				this->WriteByte(addr, STACK_ADDRESS + m_datastack);
			}
			else
			{
				m_pc += 2;
			}

			m_pc++;
			break;

		case OP_POP_A:
			// Pop the top of the stack into the accumulator //
			if (m_datastack > 0)
			{
				m_datastack--;
				m_accumulator = this->ReadByte(STACK_ADDRESS + m_datastack);
			}
			m_pc++;
			break;

		case OP_POP_Z:
			// Pop the top of the stack into the Z-index //
			if (m_datastack > 0)
			{
				m_datastack--;
				m_zindex = this->ReadByte(STACK_ADDRESS + m_datastack);
			}
			m_pc++;
			break;

		case OP_JMP:
			// Jump to a different location in the code //
			addr = (this->ReadByte(++m_pc) << 8) | this->ReadByte(++m_pc);
			m_pc = addr;
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
		}
	}

	bool CPU::LoadRom(const char* fname)
	{
		FILE* romfile = fopen(fname, "r");

		if (!romfile)
		{
			std::cout << "Unable to open file " << fname << "." << std::endl;
			return false;
		}

		int rombyte = fgetc(romfile);
		int position = 0;

		while (rombyte != EOF)
		{
			this->WriteByte(PRGROM_ADDRESS + position, rombyte);
			position++;
			rombyte = fgetc(romfile);
		}

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
}