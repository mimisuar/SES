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
		
#include "../common/opcodes.hpp"
		

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