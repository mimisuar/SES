#include "System.hpp"

namespace SES
{
	System::System()
	{
		m_cpu = new CPU();
		m_gpu = new GPU();
		//m_apu = new APU();
		m_screen = new sf::RenderWindow(sf::VideoMode(256 * 2, 240 * 2), "SES");

		m_keyboard = 0;

		// create the external file storage //
#ifdef _WIN32
		CreateDirectory(L"emem", NULL);
#endif
	}

	System::~System()
	{
		delete m_cpu;
		delete m_gpu;
		//delete m_apu;

		delete m_screen;
	}

	void System::Reset()
	{
		m_cpu->Reset();
		m_gpu->Reset();
		//m_apu->Reset();
	}

	void System::Run()
	{
		// run everything //

		while (m_screen->isOpen())
		{
			sf::Event e;
			while (m_screen->pollEvent(e))
			{
				switch (e.type)
				{
				case sf::Event::Closed:
					m_screen->close();
					break;

				case sf::Event::KeyPressed:
					switch (e.key.code)
					{
					case sf::Keyboard::BackSpace:
						// Reset //
						m_cpu->FlagEnableBit(CPU::FLAG_RESET);
						break;

					case sf::Keyboard::W:
						this->EnableKeyboard(KEYBOARD_W);
						break;

					case sf::Keyboard::A:
						this->EnableKeyboard(KEYBOARD_A);
						break;

					case sf::Keyboard::S:
						this->EnableKeyboard(KEYBOARD_S);
						break;
						
					case sf::Keyboard::D:
						this->EnableKeyboard(KEYBOARD_D);
						break;

					case sf::Keyboard::O:
						this->EnableKeyboard(KEYBOARD_O);
						break;

					case sf::Keyboard::K:
						this->EnableKeyboard(KEYBOARD_K);
						break;

					case sf::Keyboard::Return:
						this->EnableKeyboard(KEYBOARD_RETURN);
						break;

					case sf::Keyboard::LShift:
					case sf::Keyboard::RShift:
						this->EnableKeyboard(KEYBOARD_SHIFT);
						break;
					}

				case sf::Event::KeyReleased:
					switch (e.key.code)
					{
					case sf::Keyboard::W:
						this->DisableKeyboard(KEYBOARD_W);
						break;

					case sf::Keyboard::A:
						this->DisableKeyboard(KEYBOARD_A);
						break;

					case sf::Keyboard::S:
						this->DisableKeyboard(KEYBOARD_S);
						break;

					case sf::Keyboard::D:
						this->DisableKeyboard(KEYBOARD_D);
						break;

					case sf::Keyboard::O:
						this->DisableKeyboard(KEYBOARD_O);
						break;

					case sf::Keyboard::K:
						this->DisableKeyboard(KEYBOARD_K);
						break;

					case sf::Keyboard::Return:
						this->DisableKeyboard(KEYBOARD_RETURN);
						break;

					case sf::Keyboard::LShift:
					case sf::Keyboard::RShift:
						this->DisableKeyboard(KEYBOARD_SHIFT);
						break;
					}
					break;
				}
			}
			m_cpu->WriteByte(KEYBOARD_ADDR, m_keyboard);

			// update the cpu //
			m_cpu->Run();

			m_screen->clear();

			// draw data from the gpu //

			m_screen->display();

			if (m_cpu->FlagIsEnabled(CPU::FLAG_RESET))
			{
				this->Reset();
			}

			if (m_cpu->FlagIsEnabled(CPU::FLAG_POFF))
			{
				m_screen->close();
			}
		}

		m_cpu->SaveExternalMem();
	}

	bool System::LoadRom(const char* fname)
	{

		return m_cpu->LoadRom(fname);
	}

	void System::DisableKeyboard(byte keyflag)
	{
		m_keyboard &= ~(keyflag);
	}

	void System::EnableKeyboard(byte keyflag)
	{
		m_keyboard |= keyflag;
	}
}