#include <SFML\Graphics.hpp>

int main()
{
	sf::Sprite mainSpr;
	sf::Texture mainTexture;

	sf::Image images[2];
	images[0].loadFromFile("sesman.png");
	images[1].loadFromFile("test.png");

	sf::RenderWindow window(sf::VideoMode(800, 600), "Test");

	while (window.isOpen())
	{
		sf::Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		window.clear();

		mainTexture.loadFromImage(images[0]);
		mainSpr.setTexture(mainTexture, true);
		mainSpr.setPosition(0, 0);
		window.draw(mainSpr);

		mainTexture.loadFromImage(images[1]);
		mainSpr.setTexture(mainTexture, true);
		mainSpr.setPosition(100, 100);
		window.draw(mainSpr);

		window.display();
	}

	return EXIT_SUCCESS;
}