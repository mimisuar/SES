#include "GenerateChar.hpp"
#include "GeneratePalette.hpp"
#include "PaletteUtility.hpp"
#include "View.hpp"

int main(int argc, char* argv[])
{
	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "generate-palette") == 0)
		{
			// i				i + 1  i + 2   i + 3    //
			// generate-palette <mode> <input> <output> //
			if (i + 3 >= argc)
			{
				printf("gfxutil generate-palette <mode> <input> <output>\n");
				return EXIT_FAILURE;
			}

			GeneratePalette(argv[i + 1], argv[i + 2], argv[i + 3]);
			return EXIT_SUCCESS;
		}
		else if (strcmp(argv[i], "generate-char") == 0)
		{
			// i             i + 1         i + 2           i + 3
			// generate-char <input-image> <input-palette> <output> //
			if (i + 3 >= argc)
			{
				printf("gfxutil generate-char <input-image> <input-palette> <output>\n");
				return EXIT_FAILURE;
			}
			GenerateChar(argv[i + 1], argv[i + 2], argv[i + 3]);
			return EXIT_SUCCESS;
		}
		else if (strcmp(argv[i], "convert") == 0)
		{
			// i       i + 1  i + 2   i + 3 //
			// convert <mode> <input> <output> //
			if (i + 3 >= argc)
			{
				printf("gfxutil convert <mode> <input-image> <output>\n");

				return EXIT_FAILURE;
			}

			GeneratePalette(argv[i + 1], argv[i + 2], "temp.pal");
			GenerateChar(argv[i + 2], "temp.pal", argv[i + 3]);
			return EXIT_SUCCESS;
		}
		else if (strcmp(argv[i], "regenerate-palette") == 0)
		{
			Palette::Regenerate();
		}
		else if (strcmp(argv[i], "view") == 0)
		{
			if (i + 2 >= argc)
			{
				printf("gfxutil View <inputImage> <inputPalette>");
				return EXIT_FAILURE;
			}

			View(argv[i + 1], argv[i + 2]);
			return EXIT_SUCCESS;
		}
	}

	return EXIT_FAILURE;
}