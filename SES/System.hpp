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

		static const byte MOUSE_LEFT		= 0x01;
		static const byte MOUSE_RIGHT		= 0x02;
		static const byte MOUSE_MIDDLE		= 0x04;
		static const word MOUSEX_ADDR		= 0x411;
		static const word MOUSEY_ADDR		= 0x412;
		static const word MOUSE_ADDR		= 0x413;

		static const word GPU_CTRL			= 0x414;
		static const word GPU_UPPER			= 0x415;
		static const word GPU_LOWER			= 0x416;
		static const word GPU_PORT			= 0x417;

		void Reset();

		void Run();
		bool LoadRom(const char* fname);
	private:
		CPU* m_cpu;
		GPU* m_gpu;
		//APU* m_apu;
		sf::RenderWindow* m_screen; // this is the actual output screen that people see. The gpu has the data for it. //
		byte m_keyboard;
		byte m_mouse;

		void DisableKeyboard(byte keyflag);
		void EnableKeyboard(byte keyflag);
		void DisableMouse(byte mouseflag);
		void EnableMouse(byte mouseflag);
	};
}