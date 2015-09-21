#include "PaletteUtility.hpp"

#include "../common/palette_png.hpp"

namespace Palette
{
	sf::Color* Init()
	{
		sf::Image paletteImage;
		if (!paletteImage.loadFromMemory(PALETTE_PNG_HPP, PALETTE_PNG_HPP_SIZE))
		{
			printf("Something went wrong reading palette.\n");
			return nullptr;
		}

		sf::Color* palette = new sf::Color[0x41];

		unsigned int x = 0, y = 0;
		unsigned int width = paletteImage.getSize().x, height = paletteImage.getSize().y;

		for (unsigned int i = 0; i < 0x40; i++)
		{
			palette[i] = paletteImage.getPixel(x, y);
			x++;
			if (x >= width)
			{
				x = 0;
				y++;
				if (y >= height)
				{
					break;
				}
			}
		}

		palette[0x40] = sf::Color(0, 0, 0, 0);

		return palette;
	}

	bool Compare(sf::Color& color1, sf::Color& color2)
	{
		return ((color1.r == color2.r
		&& color1.g == color2.g
		&& color1.b == color2.b
		&& color1.a > 0
		&& color2.a > 0)
		|| (color1.a == 0
		&& color2.a == 0));
	}

	int GetIdFromColor(sf::Color& color)
	{
		sf::Color* palette = Init();

		if (color.a == 0)
		{
			return 0x40;
		}

		for (int i = 0; i < 0x40; i++)
		{
			if (Compare(color, palette[i]))
			{
				return i;
			}
		}

		return -1;
	}

	sf::Color GetColorFromId(int id)
	{
		return Init()[id];
	}

	void Regenerate()
	{
		sf::Color* palette = Init();
		sf::Image paletteImage;
		paletteImage.create(16, 4);

		for (unsigned int y = 0; y < 4; y++)
		{
			for (unsigned int x = 0; x < 16; x++)
			{
				paletteImage.setPixel(x, y, palette[(y * 16) + x]);
			}
		}

		paletteImage.saveToFile("palette.png");
	}
}