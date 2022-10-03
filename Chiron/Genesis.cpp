#include <iostream>
#include <SFML\Graphics.hpp>

sf::RenderWindow window(sf::VideoMode(1280, 720), "Chiron");
void runEngine();
//Engine engine;
int main()
{
	window.setFramerateLimit(60);
	runEngine();
}

void runEngine()
{
	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			//engine.run()
		}

		window.clear(sf::Color(30, 30, 30, 255));
		window.display();
	}
}