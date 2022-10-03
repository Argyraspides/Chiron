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
		
		sf::Event event;
		while (window.pollEvent(event))
		{
			handleInput(event);
		}
		window.clear(sf::Color(30, 30, 30, 255));

		if (!pause)
		{
			engine.run(polygons);

			for (Polygon& p : polygons)
			{
				// Render doesn't actually show anything to the screen, thats the job of "window.draw()".
				// It just moves the position sf::ConvexShape object in the Polygon instance.
				p.render();
			}
		}

		// Actually renders the polygon to the screen.
		for (Polygon& p : polygons)
		{
			window.draw(p.renderedShape);
		}

		for (sf::CircleShape &c : drawingPoints)
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
		else if (keyReleased && keyCode == k::V && polygons.size() > 0)
		{
			polygons.erase(polygons.begin() + polygons.size() - 1);
		}
		// Clear all shapes
		else if (keyReleased && keyCode == k::Escape && polygons.size() > 0)
		{
			polygons.clear();
		}
	}	
	else 
	{
		if (keyReleased && keyCode == k::P)
		{
			sf::CircleShape circle(3.0f);
			circle.setPosition(mouse.x, mouse.y);
			drawingPoints.push_back(circle);
		} 
		else if (keyReleased && keyCode == k::D)
		{
			drawingPoints.erase(drawingPoints.begin() + drawingPoints.size() - 1);
		}
		else if (keyReleased && keyCode == k::Enter)
		{
			makeShape();
			pause = !pause;
		}
	}

	if (keyReleased && keyCode == k::SemiColon) 
	{
		pause = !pause;
		std::cout << "Pause: " << pause << std::endl;
	}
}

void makeShape()
{
	// We are done drawing our polygon. We will take the positions of all the
	// dots on the screen and put them into vertices.

	std::vector<Vertex> vertices(drawingPoints.size());
	sf::ConvexShape convexShape;

	// We must define these before actually giving our convexShape any features ...
	// Thanks SFML.
	convexShape.setFillColor(sf::Color(rand() * rand() * rand()));
	convexShape.setPointCount(drawingPoints.size());
	convexShape.setOutlineThickness(1.0f);

	int i = -1;
	for (sf::CircleShape& c : drawingPoints)
	{
		Vertex v = { c.getPosition().x, c.getPosition().y };
		std::cout << "(" << c.getPosition().x << ", " << c.getPosition().y << ")" << std::endl;
		vertices[++i] = v;
		convexShape.setPoint(i, sf::Vector2f(v.x, v.y));
	}
	drawingPoints.clear();

	Polygon poly(0, 0, 1, vertices);
	poly.center.y *= -1;
	poly.vel = { 1,1 };
	poly.ang_vel = .005f;
	std::cout << "(" << poly.center.x << ", " << poly.center.y << ")" << std::endl;
	convexShape.setOrigin(poly.center.x, poly.center.y);
	convexShape.setPosition(poly.center.x, poly.center.y);

	poly.renderedShape = convexShape;
	polygons.push_back(poly);
}

	

