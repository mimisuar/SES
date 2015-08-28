#pragma once

#include "CPU.hpp"

namespace SES
{
	class CPU;

	class GPU
	{
	public:
		GPU();
		~GPU();

		void Reset();

		// Constants //
		static const unsigned int MEMORY_SIZE = 0x1000;

		void WriteByte(word addr, byte val);
		byte ReadByte(word addr);
	private:
		byte m_vram[MEMORY_SIZE];
	};
}