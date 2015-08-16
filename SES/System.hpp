#pragma once
#include <SFML\Graphics.hpp>
#include "CPU.hpp"

namespace SES
{
	// This class represents the actual hardware of the SES. It actually powers on and resets the different components //
	class System
	{
	public:
		System();
		~System();

		void Reset();

		void Run();
		bool LoadRom(const char* fname);
	private:
		CPU* m_cpu;
		//GPU* m_gpu;
		//APU* m_apu;
		sf::RenderWindow* m_screen; // this is the actual output screen that people see. The gpu has the data for it. //
	};
}