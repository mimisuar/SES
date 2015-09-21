#pragma once

#include <SFML\Graphics\Color.hpp>
#include <SFML\Graphics\Image.hpp>

namespace Palette 
{
	sf::Color* Init();
	bool Compare(sf::Color& color1, sf::Color& color2);
	int GetIdFromColor(sf::Color& color);
	sf::Color GetColorFromId(int id);
	void Regenerate();
}