#include <iostream>
#include <SFML\Graphics.hpp>

sf::RenderWindow window(sf::VideoMode(1280, 720), "Chiron");

int main()
{


	while (window.isOpen())
	{
		sf::Event event;

		//while (window.pollEvent(event))
			
		window.clear(sf::Color(30, 30, 30, 255));

		window.display();
	}


	return (0);

}