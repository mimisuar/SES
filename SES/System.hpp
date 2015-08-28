#pragma once
#include <SFML\Graphics.hpp>
#include "CPU.hpp"
#include "GPU.hpp"

#ifdef _WIN32
#include <Windows.h>
#else
#error This program is only compatible with Win32.
#endif


namespace SES
{
	// This class represents the actual hardware of the SES. It actually powers on and resets the different components //
	class System
	{
	public:
		System();
		~System();
		
		// Constants //
		static const byte KEYBOARD_W		= 0x80;
		static const byte KEYBOARD_A		= 0x40;
		static const byte KEYBOARD_S		= 0x20;
		static const byte KEYBOARD_D		= 0x10;
		static const byte KEYBOARD_O		= 0x08;
		static const byte KEYBOARD_K		= 0x04;
		static const byte KEYBOARD_RETURN	= 0x02;
		static const byte KEYBOARD_SHIFT	= 0x01;
		static const word KEYBOARD_ADDR		= 0x0410;

		void Reset();

		void Run();
		bool LoadRom(const char* fname);
	private:
		CPU* m_cpu;
		GPU* m_gpu;
		//APU* m_apu;
		sf::RenderWindow* m_screen; // this is the actual output screen that people see. The gpu has the data for it. //
		byte m_keyboard;

		void DisableKeyboard(byte keyflag);
		void EnableKeyboard(byte keyflag);
	};
}