#include "GenesisHeaders.h"

bool pause = false;


int main()
{
	window.setFramerateLimit(60);
	runEngine();
}
void runEngine()
{
	while (window.isOpen())
	{
		window.clear(sf::Color(30, 30, 30, 255));

		sf::Event event;
		while (window.pollEvent(event))
		{
			handleInput(event);
		}

		if (!pause)
		{
			engine.run(polygons);
		}

		for (Polygon p : polygons)
		{
			p.render();
			window.draw(p.renderedShape);
		}

		for (sf::CircleShape c : drawingPoints)
		{
			window.draw(c);
		}

		window.display();
	}
}
void handleInput(sf::Event& event)
{
#define mouse sf::Mouse::getPosition(window)
#define keyReleased event.type == sf::Event::KeyReleased
#define k sf::Keyboard
#define keyCode event.key.code

	if (!pause)
	{
		// Spawn an arbitrary polygon
		if (keyReleased && keyCode == k::A)
		{
		}
		// Remove the most recently added shape
		if (keyReleased && keyCode == k::V && polygons.size() > 0)
		{
			polygons.erase(polygons.begin() + polygons.size() - 1);
		}
		// Clear all shapes
		if (keyReleased && keyCode == k::Escape && polygons.size() > 0)
		{
			polygons.clear();
		}
		
	}
	else 
	{
		if (keyReleased && keyCode == k::P)
		{
			sf::CircleShape circle(5);
			circle.setPosition(mouse.x, mouse.y);
			drawingPoints.push_back(circle);
		}
		if (keyReleased && keyCode == k::D)
		{
			drawingPoints.erase(drawingPoints.begin() + drawingPoints.size() - 1);
		}
		if (keyReleased && keyCode == k::Enter)
		{
			std::vector<Vertex> vertices;
			sf::ConvexShape convexShape;

			convexShape.setFillColor(sf::Color(rand() * rand() * rand()));
			convexShape.setPointCount(4);
			convexShape.setOutlineThickness(1.0f);

			vertices.reserve(drawingPoints.size());
			int i = 0;
			for (sf::CircleShape c : drawingPoints)
			{
				Vertex v = { c.getPosition().x, c.getPosition().y };
				vertices.push_back(v);
				convexShape.setPoint(i, sf::Vector2f(v.x, v.y));

				i++;
			}

			drawingPoints.clear();

			Polygon p(0, 0, 1, vertices);
			p.renderedShape = convexShape;
			polygons.push_back(p);
			pause = !pause;
		}
	}

	if (keyReleased && keyCode == k::SemiColon) 
	{
		pause = !pause;
		std::cout << "Pause: " << pause << std::endl;
	}
}

	

