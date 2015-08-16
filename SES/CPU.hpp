#pragma once

namespace SES
{

	typedef unsigned char byte;
	typedef unsigned short word;

	class CPU
	{
	public:
		CPU();
		~CPU();

		void Reset();

		// Constants //
		static const unsigned int MEMORY_SIZE = 0x1000;
		static const word PRGROM_ADDRESS = 0x250;
		static const byte FLAG_OVERFLOW = 1;
		static const byte FLAG_EQUAL = 2;
		static const byte FLAG_GREATER = 4;
		static const byte FLAG_LESS = 8;
		static const byte FLAG_1 = 16;
		static const byte FLAG_2 = 32;
		static const byte FLAG_3 = 64;
		static const byte FLAG_4 = 128;
		static const word STACK_ADDRESS = 0x200;
		// Opcodes //
		// Store / Load //
		static const byte OP_LDA_ADDR = 0x0F;
		static const byte OP_LDA_BYTE = 0x5F;
		static const byte OP_LDA_Z = 0x7F;
		static const byte OP_LDZ_ADDR = 0x1F;
		static const byte OP_LDZ_BYTE = 0x6F;
		static const byte OP_LDZ_A = 0x8F;
		static const byte OP_STA_ADDR = 0x2F;
		static const byte OP_STZ_ADDR = 0x3F;
		static const byte OP_STF_ADDR = 0x4F;
		// Stack //
		static const byte OP_PUSH_BYTE = 0x0A;
		static const byte OP_PUSH_ADDR = 0x1A;
		static const byte OP_PUSH_A = 0x2A;
		static const byte OP_PUSH_Z = 0x3A;
		static const byte OP_PUSH_F = 0x4A;
		static const byte OP_POP_ADDR = 0x0B;
		static const byte OP_POP_A = 0x1B;
		static const byte OP_POP_Z = 0x2B;
		// Branching //
		static const byte OP_RET = 0x00;
		static const byte OP_CALL = 0x10;
		static const byte OP_JMP = 0x20;
		// Math //
		static const byte OP_ADD_BYTE = 0x0C;
		static const byte OP_ADD_Z = 0x2C;
		static const byte OP_SUB_BYTE = 0x1C;
		static const byte OP_SUB_Z = 0x3C;
		

		void WriteByte(word address, byte value);
		byte ReadByte(word address);

		void Run();

		bool LoadRom(const char* fname);

		void FlagEnableBit(byte mask = 0xFF);
		void FlagDisableBit(byte mask = 0xFF);
		void FlagToggleBit(byte mask = 0xFF);
	private:
		byte m_ram[MEMORY_SIZE];
		byte m_accumulator;
		byte m_zindex;
		byte m_datastack;
		byte m_callstack[16];
		byte m_statusflag;
		word m_pc;
		byte m_callsp;
	};
}