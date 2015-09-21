#pragma once

#include <iostream>

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
		static const unsigned int MEMORY_SIZE			= 0x1000;
		static const unsigned int EXTERNAL_MEMORY_SIZE	= 0x100;
		static const word PRGROM_ADDRESS				= 0x500;
		static const byte FLAG_OVERFLOW					= 1;
		static const byte FLAG_EQUAL					= 2;
		static const byte FLAG_GREATER					= 4;
		static const byte FLAG_LESS						= 8;
		static const byte FLAG_RESET					= 16;
		static const byte FLAG_POFF						= 32;
		static const byte FLAG_GPU						= 64;
		static const byte FLAG_APU						= 128;
		static const word STACK_ADDRESS					= 0x400;
		static const word GPU_PORT						= 0x417;
		// Opcodes //
		// Store / Load //
		static const byte OP_LDA_ADDR	= 0x0F;
		static const byte OP_LDA_BYTE	= 0x5F;
		static const byte OP_LDA_Z		= 0x7F;
		static const byte OP_LDA_I		= 0x02;
		static const byte OP_LDZ_ADDR	= 0x1F;
		static const byte OP_LDZ_BYTE	= 0x6F;
		static const byte OP_LDZ_A		= 0x8F;
		static const byte OP_LDZ_I		= 0x12;
		static const byte OP_LDI_ADDR	= 0x4F;
		static const byte OP_LDI_IADDR	= 0xCF;
		static const byte OP_STA_ADDR	= 0x2F;
		static const byte OP_STZ_ADDR	= 0x3F;
		static const byte OP_STI_ADDR	= 0xBF;
		// Stack //
		static const byte OP_PUSH_BYTE	= 0x0A;
		static const byte OP_PUSH_ADDR	= 0x1A;
		static const byte OP_PUSH_A		= 0x2A;
		static const byte OP_PUSH_Z		= 0x3A;
		static const byte OP_PUSH_I		= 0x22;
		static const byte OP_POP_ADDR	= 0x0B;
		static const byte OP_POP_A		= 0x1B;
		static const byte OP_POP_Z		= 0x2B;
		static const byte OP_POP_I		= 0x32;
		// Branching //
		static const byte OP_RET		= 0x00;
		static const byte OP_CALL		= 0x10;
		static const byte OP_CALL_I		= 0x42;
		static const byte OP_JMP		= 0x20;
		static const byte OP_JMP_I		= 0x52;
		static const byte OP_CPA_ADDR	= 0x30;
		static const byte OP_CPA_BYTE	= 0x40;
		static const byte OP_CPA_I		= 0x62;
		static const byte OP_CPZ_ADDR	= 0x50;
		static const byte OP_CPZ_BYTE	= 0x60;
		static const byte OP_CPZ_I		= 0x72;
		static const byte OP_BEQ_ADDR	= 0x70;
		static const byte OP_BEQ_I		= 0x82;
		static const byte OP_BNE_ADDR	= 0x80;
		static const byte OP_BNE_I		= 0x92;
		static const byte OP_BGT_ADDR	= 0x90;
		static const byte OP_BGT_I		= 0xA2;
		static const byte OP_BLT_ADDR	= 0xA0;
		static const byte OP_BLT_I		= 0xB2;
		static const byte OP_POFF		= 0xB0;
		static const byte OP_RST		= 0xC0;
		// Math //
		static const byte OP_ADD_BYTE	= 0x0C;
		static const byte OP_ADD_Z		= 0x2C;
		static const byte OP_SUB_BYTE	= 0x1C;
		static const byte OP_SUB_Z		= 0x3C;
		static const byte OP_LSH_BYTE	= 0x4C;
		static const byte OP_LSH_Z		= 0xAC;
		static const byte OP_RSH_BYTE	= 0x5C;
		static const byte OP_RSH_Z		= 0xBC;
		static const byte OP_AND_BYTE	= 0x7C;
		static const byte OP_AND_Z		= 0xDC;
		static const byte OP_OR_BYTE	= 0x8C;
		static const byte OP_OR_Z		= 0xEC;
		static const byte OP_XOR_BYTE	= 0x9C;
		static const byte OP_XOR_Z		= 0xFC;
		static const byte OP_ADI_ADDR	= 0x03;
		static const byte OP_SBI_ADDR	= 0x13;
		// Flag //
		static const byte OP_EOF		= 0x01;
		static const byte OP_EEF		= 0x11;
		static const byte OP_EGF		= 0x21;
		static const byte OP_ELF		= 0x31;
		static const byte OP_EGPUF		= 0x41;
		static const byte OP_DOF		= 0x81;
		static const byte OP_DEF		= 0x91;
		static const byte OP_DGF		= 0xA1;
		static const byte OP_DLF		= 0xB1;
		static const byte OP_DGPUF		= 0xC1;
		// File //
		static const byte OP_WRA_ADDR	= 0x9F;
		static const byte OP_RDA_ADDR	= 0xAF;
		static const byte OP_WRA_I		= 0xC2;
		static const byte OP_RDA_I		= 0xD2;
		

		void WriteByte(word address, byte value);
		byte ReadByte(word address);

		void PushStack(byte value);
		byte PopStack();
		
		// External Memory //
		void InitExternalMem(const char* fname);
		void SaveExternalMem();

		void Run();

		bool LoadRom(const char* fname);

		void FlagEnableBit(byte mask = 0xFF);
		void FlagDisableBit(byte mask = 0xFF);
		void FlagToggleBit(byte mask = 0xFF);
		bool FlagIsEnabled(byte mask = 0xFF);
	private:
		byte m_ram[MEMORY_SIZE];
		byte m_emem[EXTERNAL_MEMORY_SIZE];
		byte m_accumulator;
		byte m_zindex;
		word m_index;
		byte m_datastack;
		word m_callstack[16];
		byte m_statusflag;
		word m_pc;
		byte m_callsp;
		const char* m_ememfile;
		std::string m_ememfile_str;
	};
}