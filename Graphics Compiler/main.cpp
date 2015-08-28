#include <SFML\Graphics.hpp>
#include <initializer_list>

bool AreColorsEqual(sf::Color& color1, sf::Color& color2);

void Convert(char* input, char* output);
void Compile(char* output, char*...);

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		goto INFO;
	}

	if (strcmp(argv[1], "convert") == 0)
	{
		// convert from .png to a .gfx //
		printf("Converting from %s to %s.\n", argv[2], argv[3]);
		Convert(argv[2], argv[3]);
		return EXIT_SUCCESS;
	}
	else if (strcmp(argv[1], "compile") == 0)
	{
		// compile all given files into a single one //
		printf("Compiling given files...");
	}

INFO:
	printf("gfxcompiler <cmd> <args...>");
	return EXIT_FAILURE;
}

bool AreColorsEqual(sf::Color& color1, sf::Color& color2)
{
	return (color1.r == color2.r
		&&	color1.g == color2.g
		&&	color1.b == color2.b)
		|| (color1.a == color2.a
		&& color1.a == 0x00);
}

void Convert(char* input, char* output)
{
	sf::Image image;
	if (!image.loadFromFile("palette.png"))
	{
		return;
	}

	// load all of the colors //
	sf::Color colors[0x40];
	unsigned int x = 0, y = 0;
	for (unsigned int i = 0; i < 0x40; i++)
	{
		colors[i] = image.getPixel(x, y);
		
		x++;
		if (x > 0xF)
		{
			x = 0;
			y++;
			if (y > 0x3)
			{
				break;
			}
		}
	}

	if (!image.loadFromFile(input))
	{
		return;
	}

	// go through all the colors of the actual picture we want to convert //
	unsigned char picturePalette[4] = { 0x40 }; // 0x40 = Alpha //
	unsigned char firstEmpty = 0;
	// Palette: XX
	// Upper:   ^|
	// Lower:    ^
	unsigned char patternsUpper[8][8] = { 0 };
	unsigned char patternsLower[8][8] = { 0 };

	for (unsigned int j = 0; j < 8; j++)
	{
		for (unsigned int i = 0; i < 8; i++)
		{
			sf::Color color = image.getPixel(i, j);
		}
	}
}

void Compile(std::initializer_list<char*> files, char* output)
{

}