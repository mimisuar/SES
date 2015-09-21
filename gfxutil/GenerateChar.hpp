#pragma once
#include "PaletteUtility.hpp"

void GenerateChar(char* inputImage, char* inputPalette, char* output)
{
	// Read the palette //
	FILE* inputPaletteFile;
	unsigned char imagePalette[4] = { 0x40 };
	if (fopen_s(&inputPaletteFile, inputPalette, "rb") != 0)
	{
		printf("Unable to open palette file %s.\n", inputPalette);
		return;
	}
	fread_s(imagePalette, 4, 1, 4, inputPaletteFile);
	fclose(inputPaletteFile);

	sf::Image image;
	if (!image.loadFromFile(inputImage))
	{
		printf("Unable to open image file %s.\n", inputImage);
		return;
	}

	unsigned int width = image.getSize().x, height = image.getSize().y;

	if (width % 8 != 0 || height % 8 != 0)
	{
		printf("Invalid tile size. Each side must be divisble by 8.\n");
		return;
	}

	unsigned char horizontalTiles = width / 8, verticalTiles = height / 8;

	unsigned char** upperBits = new unsigned char*[horizontalTiles * verticalTiles];
	unsigned char** lowerBits = new unsigned char*[horizontalTiles * verticalTiles];

	
	for (int i = 0; i < horizontalTiles * verticalTiles; i++)
	{
		upperBits[i] = new unsigned char[8];
		lowerBits[i] = new unsigned char[8];

		for (int j = 0; j < 8; j++)
		{
			upperBits[i][j] = 0;
			lowerBits[i][j] = 0;
		}
	}

	for (unsigned char ty = 0; ty < verticalTiles; ty++)
	{
		for (unsigned char tx = 0; tx < horizontalTiles; tx++)
		{
			// please forgive me //
			unsigned char currentTile = (ty * horizontalTiles) + tx;

			for (unsigned char y = 0; y < 8; y++)
			{
				for (unsigned char x = 0; x < 8; x++)
				{
					sf::Color currentColor = image.getPixel((tx * 8) + x, (ty * 8) + y);

					for (unsigned char i = 0; i < 4; i++)
					{
						if (Palette::Compare(currentColor, Palette::GetColorFromId(imagePalette[i])))
						{
							// mark this number //
							upperBits[currentTile][y] |= ((i & 2) >> 1) << (7 - x);
							lowerBits[currentTile][y] |= (i & 1) << (7 - x);
							break;
						}
					}
				}
			}
		}
	}

	// Now we write the results to a file //
	FILE* outputFile;
	if (fopen_s(&outputFile, output, "wb") != 0)
	{
		printf("Unable to open file for writing.");
		return;
	}

	char toWrite[2] = { 0 };

	for (unsigned int ty = 0; ty < verticalTiles; ty++)
	{
		for (unsigned int tx = 0; tx < horizontalTiles; tx++)
		{
			unsigned char currentTile = (ty * horizontalTiles + tx);

			for (int i = 0; i < 8; i++)
			{
				toWrite[0] = upperBits[currentTile][i];
				toWrite[1] = lowerBits[currentTile][i];
				fwrite(toWrite, 1, 2, outputFile);
			}
		}
	}

	fclose(outputFile);
}