#include "GPU.hpp"

#include "../common/palette_png.hpp"

namespace SES
{
	GPU::GPU()
	{
		for (int i = 0; i < MEMORY_SIZE; i++)
		{
			m_vram[i] = 0;
		}

		// create the palette //
		sf::Image paletteImage;
		m_palette[0x40] = sf::Color(0, 0, 0, 0);
		for (int i = 0; i < 0x40; i++)
		{
			m_palette[i] = sf::Color(255, 255, 255);
		}

		if (!paletteImage.loadFromMemory(PALETTE_PNG_HPP, PALETTE_PNG_HPP_SIZE))
		{
			printf("Unable to load palette. Colors will only be white.\n");
			return;
		}

		m_canvasTexture.create(256, 240);
		
		// initilize the sprite tiles //

		unsigned int width = paletteImage.getSize().x, height = paletteImage.getSize().y;

		for (unsigned int y = 0; y < height; y++)
		{
			for (unsigned int x = 0; x < width; x++)
			{
				m_palette[(y * width) + x] = paletteImage.getPixel(x, y);
			}
		}
	}

	GPU::~GPU()
	{

	}

	void GPU::Reset()
	{

	}

	void GPU::WriteByte(word address, byte value)
	{
		if (address < MEMORY_SIZE)
		{
			m_vram[address] = value;
		}
	}

	byte GPU::ReadByte(word address)
	{
		if (address < MEMORY_SIZE)
		{
			return m_vram[address];
		}

		return 0;
	}

	sf::Sprite& GPU::Draw()
	{
		m_canvasTexture.clear();


		
		m_canvasTexture.display();

		m_canvas.setTexture(m_canvasTexture.getTexture());
		m_canvas.setScale(2, 2);
		return m_canvas;
	}

	bool GPU::BitIsEnabled(byte source, byte mask)
	{
		return (source & mask) == mask;
	}
	
	
	sf::Color GPU::GetPaletteColor(byte paletteId, byte paletteNumber)
	{
		return m_palette[this->ReadByte(PALETTES_ADDRESS + (paletteId * 4) + paletteNumber)];
	}

	void GPU::DrawSprite(word spriteAddress, byte x, byte y, sf::Color& palette, bool flipX, bool flipY, bool priority)
	{
		
	}
}