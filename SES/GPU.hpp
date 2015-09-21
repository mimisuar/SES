#pragma once

#include "CPU.hpp"
#include <SFML\Graphics.hpp>

namespace SES
{
	class CPU;

	struct Character
	{
		sf::Image image;
		sf::Texture texture;
		sf::Sprite sprite;
	};

	struct SpriteData
	{
		byte xPosition = 0;
		byte yPosition = 0;
		byte attributes = 0;
		byte spriteId = 0;
		bool _inited = false;
	};

	class GPU
	{
	public:
		GPU();
		~GPU();

		void Reset();

		// Constants //
		static const unsigned int MEMORY_SIZE		= 0x1000;
		static const word SPRITES_ADDRESS			= 0x000;
		static const word BACKGROUNDS_ADDRESS		= 0x300;
		static const word PALETTES_ADDRESS			= 0x600;
		static const word OAM_ADDRESS				= 0x640;
		static const word NAMETABLE_ADDRESS			= 0x740;
		static const word SPRITES_TOTAL				= BACKGROUNDS_ADDRESS / 16;
		static const word BACKGROUNDS_TOTAL			= BACKGROUNDS_ADDRESS / 16;
		static const word OAM_TOTAL					= (NAMETABLE_ADDRESS - OAM_ADDRESS) / 4;

		static const byte SPR_ENABLED				= 0x80;
		static const byte SPR_FLIPX					= 0x40;
		static const byte SPR_FLIPY					= 0x20;
		static const byte SPR_PRIORITY				= 0x10;

		static const word CTRL_INCVAL				= 0x03;

		static const unsigned int SCREEN_WIDTH = 256;
		static const unsigned int SCREEN_HEIGHT = 240;

		void WriteByte(word addr, byte val);
		byte ReadByte(word addr);

		sf::Sprite& Draw();

		sf::Color GetPaletteColor(byte paletteId, byte paletteNumber);
		void DrawSprite(word spriteAddress, byte x, byte y, sf::Color& palette, bool flipX, bool flipY, bool priority);

		bool BitIsEnabled(byte source, byte mask);
	private:
		byte m_vram[MEMORY_SIZE];
		sf::Color m_palette[0x41];

		sf::RenderTexture m_canvasTexture;
		sf::Sprite m_canvas;
		byte m_screen[SCREEN_WIDTH][SCREEN_HEIGHT];
	};
}