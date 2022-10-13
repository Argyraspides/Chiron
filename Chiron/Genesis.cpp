#include "GenesisFunctions.h"
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

		if (!pause)
		{
			window.clear(sf::Color(30, 30, 30, 255));
		}
		else
		{
			window.clear(sf::Color(75, 80, 84, 255));
		}

		if (!pause)
		{
			engine.run(polygons);
		}

		// Actually renders the polygon to the screen.
		for (Polygon& p : polygons)
		{
			window.draw(p.renderedShape);
		}

		for (sf::CircleShape& c : drawingPoints)
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
		else if (keyReleased && keyCode == k::C)
		{
			system("CLS");
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
		else if (keyReleased && keyCode == k::C)
		{
			// Position of the circles center
			Vertex circleCenter = { mouse.x, mouse.y };
			// The circle will be approximated by this many vertices
			int approximationFactor = 13;
			int circleRadius = 30;

			// The angle to change by on each iteration. 2pi radians is one full rotation.
			float rotIncrements = (2 * 3.14159265359 / approximationFactor);
			
			for (int i = 0; i < approximationFactor; i++)
			{
				float theta = rotIncrements * i;
				float xRot = circleRadius * cos(theta) - circleRadius * sin(theta);
				float yRot = circleRadius * sin(theta) + circleRadius * cos(theta);
				sf::CircleShape c(3);
				c.setPosition(xRot + circleCenter.x, yRot + circleCenter.y);
				drawingPoints.push_back(c);
			}
		}
		else if (keyReleased && keyCode == k::S)
		{
			// Position of the squares center
			Vertex squareCenter = { mouse.x, mouse.y };
			// The circle will be approximated by this many vertices
			int approximationFactor = 4;
			int circleRadius = 30;

			// The angle to change by on each iteration. 2pi radians is one full rotation.
			float rotIncrements = (2 * 3.14159265359 / approximationFactor);

			for (int i = 0; i < approximationFactor; i++)
			{
				float theta = rotIncrements * i;
				float xRot = circleRadius * cos(theta) - circleRadius * sin(theta);
				float yRot = circleRadius * sin(theta) + circleRadius * cos(theta);
				sf::CircleShape c(3);
				c.setPosition(xRot + squareCenter.x, yRot + squareCenter.y);
				drawingPoints.push_back(c);
			}
		}
	}

	if (keyReleased && keyCode == k::SemiColon)
	{
		pause = !pause;
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
		vertices[++i] = v;
		convexShape.setPoint(i, sf::Vector2f(v.x, v.y));
	}
	drawingPoints.clear();
	Polygon poly(0, 0, 1, vertices);
	poly.center.y *= -1;
	float x = rand() % 2;
	float y = rand() % 2;

	poly.vel = { x,  y };

	poly.ang_vel = 0.01f;
	convexShape.setOrigin(poly.center.x, poly.center.y);
	convexShape.setPosition(poly.center.x, poly.center.y);
	poly.renderedShape = convexShape;
	polygons.push_back(poly);
}
	

