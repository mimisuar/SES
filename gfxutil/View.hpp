#pragma once

#include "PaletteUtility.hpp"
#include <SFML\Graphics.hpp>

void View(char* inputImage, char* inputPalette)
{
	sf::RenderWindow window(sf::VideoMode(256, 256), "View", sf::Style::Close);
	FILE* tmpFile;

	// read the palette //
	if (fopen_s(&tmpFile, inputPalette, "rb") != 0)
	{
		printf("Unable to open image file %s.\n", inputPalette);
		return;
	}

	// palette is only 4 bytes long //
	unsigned char palette[4];

	fread(palette, 1, 4, tmpFile);
	fclose(tmpFile);

	unsigned char backgroundColor = 0;

	// Read the image //
	

	if (fopen_s(&tmpFile, inputImage, "rb") != 0)
	{
		printf("Unable to open image file %s.\n", inputImage);
		return;
	}

	fseek(tmpFile, 0, SEEK_END);
	int fileSize = ftell(tmpFile);
	rewind(tmpFile);

	int numberOfTiles = fileSize / 16;
	char toRead[2];
	sf::Image* image = new sf::Image[numberOfTiles];

	int currentImage = 0;

	for (int i = 0; i < numberOfTiles; i++)
	{
		image[i].create(8, 8);
		for (int y = 0; y < 8; y++)
		{
			fread(toRead, 1, 2, tmpFile);

			for (int x = 0; x < 8; x++)
			{
				char upperBit = (toRead[0] >> (7 - x)) & 1;
				char lowerBit = (toRead[1] >> (7 - x)) & 1;
				char bit = (upperBit << 1) | (lowerBit);
				image[i].setPixel(x, y, Palette::GetColorFromId(palette[bit]));
			}
		}
	}

	sf::Texture tempTexture;
	tempTexture.loadFromImage(image[currentImage]);

	sf::Sprite spr;
	spr.setTexture(tempTexture);
	spr.setScale(sf::Vector2f(32, 32));

	fclose(tmpFile);

	while (window.isOpen())
	{
		sf::Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
			{
				window.close();
			}

			if (e.type == sf::Event::KeyPressed)
			{
				if (e.key.code == sf::Keyboard::Left)
				{
					currentImage--;
					if (currentImage < 0)
					{
						currentImage = numberOfTiles - 1;
					}
				}
				else if (e.key.code == sf::Keyboard::Right)
				{
					currentImage++;
					if (currentImage >= numberOfTiles)
					{
						currentImage = 0;
					}
				}
				else if (e.key.code == sf::Keyboard::Up)
				{
					backgroundColor++;
					if (backgroundColor > 0x40)
					{
						backgroundColor = 0x00;
					}
				}
				else if (e.key.code == sf::Keyboard::Down)
				{
					backgroundColor--;
					if (backgroundColor > 0x40)
					{
						backgroundColor = 0x40;
					}
				}

				tempTexture.loadFromImage(image[currentImage]);

			}
		}

		window.clear(Palette::GetColorFromId(backgroundColor));

		window.draw(spr);

		window.display();
	}
}