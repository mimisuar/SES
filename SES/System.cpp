#include "System.hpp"

namespace SES
{
	System::System()
	{
		m_cpu = new CPU();
		//m_gpu = new GPU();
		//m_apu = new APU();
		m_screen = new sf::RenderWindow(sf::VideoMode(256 * 2, 240 * 2), "SES");
	}

	System::~System()
	{
		delete m_cpu;
		//delete m_gpu;
		//delete m_apu;

	}

	void System::Reset()
	{
		m_cpu->Reset();
		//m_gpu->Reset();
		//m_apu->Reset();
	}

	void System::Run()
	{
		// run everything //

		bool reseting = false;

		while (m_screen->isOpen())
		{
			sf::Event e;
			while (m_screen->pollEvent(e))
			{
				switch (e.type)
				{
				case e.Closed:
					m_screen->close();
					break;

				case e.KeyPressed:
					switch (e.key.code)
					{
					case sf::Keyboard::BackSpace:
						// Reset //
						reseting = true;
						break;
					}
					break;
				}
			}

			// update the cpu //
			m_cpu->Run();

			m_screen->clear();

			// draw data from the gpu //

			m_screen->display();

			if (reseting)
			{
				reseting = false;
				this->Reset();
			}
		}
	}

	bool System::LoadRom(const char* fname)
	{
		return m_cpu->LoadRom(fname);
	}
}