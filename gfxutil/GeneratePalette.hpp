#pragma once
#include "PaletteUtility.hpp"

void GeneratePalette(char* mode, char* input, char* output)
{
	sf::Color* systemPalette = Palette::Init();
	
	sf::Image inputImage;

	if (!inputImage.loadFromFile(input))
	{
		printf("Unable to load %s.", input);
		return;
	}

	unsigned int width = inputImage.getSize().x, height = inputImage.getSize().y;

	if (width % 8 != 0 || height % 8 != 0)
	{
		printf("Invalid tile size. Each side must be divisble by 8.\n");
		return;
	}

	unsigned char imagePalette[4] = { 0 };
	unsigned char placesUsed = 0;

	if (strcmp(mode, "sprite") == 0)
	{
		imagePalette[0] = Palette::GetIdFromColor(sf::Color(0, 0, 0, 0));
		placesUsed++;
	}
	else if (strcmp(mode, "background") != 0)
	{
		printf("Invalid mode %s (only sprite or background).", mode);
		return;
	}
	
	for (unsigned int y = 0; y < 8; y++)
	{
		for (unsigned int x = 0; x < 8; x++)
		{
			sf::Color currentPixel = inputImage.getPixel(x, y);

			for (int i = 0; i <= 4; i++)
			{
				if (Palette::Compare(currentPixel, Palette::GetColorFromId(imagePalette[i])))
				{
					break;
				}

				if (i == placesUsed)
				{
					if (placesUsed < 4)
					{
						imagePalette[placesUsed] = Palette::GetIdFromColor(currentPixel);
						if (imagePalette[placesUsed] > 0x40)
						{
							printf("Invalid color.\n");
							return;
						}

						placesUsed++;
						break;
					}
					else
					{
						printf("Too many colors used! You can only use 3 in a sprite!");
						break;
					}
				}
			}

			if (placesUsed == 4)
			{
				break;
			}
		}

		if (placesUsed == 4)
		{
			break;
		}
	}

	// write all the data to a file //
	FILE* file;
	if (fopen_s(&file, output, "w") != 0)
	{
		printf("Unable to open %s for writing.\n", output);
		return;
	}

	fwrite(imagePalette, 1, 4, file);

#ifdef _DEBUG
	// PURELY FOR DEMO
	sf::Image demo;
	demo.create(2, 2, sf::Color(0, 0, 0, 0));

	char x = 0, y = 0;
	for (int i = 0; i < 4; i++)
	{
		demo.setPixel(x, y, Palette::GetColorFromId(imagePalette[i]));
		x++;
		if (x > 1)
		{
			x = 0;
			y++;
		}
	}

	demo.saveToFile("demo.png");
#endif


	fclose(file);
}