#include "Polygon.h"

Polygon::Polygon(float xInit, float yInit, float mass, std::vector<Vertex> &vertices)
{
	this->center = { xInit, yInit };
	this->mass = mass;
	this->vertices = vertices;
	findCentroid(vertices);
}

void Polygon::update()
{
	this->center = this->center + vel;
	rotate();
}

void Polygon::rotate()
{

	/*
		 Formula for a point's coordinates after rotation around the center for:

		 x' = x * cos(t) - y * sin(t),
		 y' = y * cos(t) + x * sin(t)

		 To rotate about the point (shape.center.x, shape.center.y), we move the points so they are
		 relative to the origin (0,0), rotate them about the origin, and move them back the same amount.
	*/

	for (Vertex &v : vertices)
	{	
		v = v + vel - center;

		float sRot = sin(ang_vel);
		float cRot = cos(ang_vel);
		
		Vertex shift =
		{
			v.x * cRot - v.y * sRot,
			v.x * sRot + v.y * cRot
		};

		v = shift + center;
	}

}

void Polygon::render()
{
	renderedShape.setPosition(this->center.x, this->center.y);
	/*
	* SFML's .rotate() takes in degrees, so we make the conversion here.
	* Degrees = Radians * (180/PI)
	*/ 
#define PI 3.141592653589793238462643383279
	renderedShape.rotate(ang_vel * (180.0f / PI));
}

void Polygon::findCentroid(std::vector<Vertex> vertices)
{
	float cx = 0, cy = 0;
	float signedArea = 0;
	Vertex center;
	float x0 = 0; float y0 = 0;
	float x1 = 0; float y1 = 0;
	float incrementedSignedArea = 0;

	int i = 0;
	for (i = 0; i < vertices.size() - 1; ++i)
	{
		x0 = vertices[i].x;
		y0 = vertices[i].y;

		x1 = vertices[i + 1].x;
		y1 = vertices[i + 1].y;

		incrementedSignedArea = x0 * y1 - x1 * y0;

		signedArea += incrementedSignedArea;

		center.x += (x0 + x1) * incrementedSignedArea;
		center.y += (y0 + y1) * incrementedSignedArea;
	}

	x0 = vertices[i].x;
	y0 = vertices[i].y;
	x1 = vertices[0].x;
	y1 = vertices[0].y;

	incrementedSignedArea = x0 * y1 - x1 * y0;
	signedArea += incrementedSignedArea;
	center.x += (x0 + x1) * incrementedSignedArea;
	center.y += (y0 + y1) * incrementedSignedArea;

	signedArea *= 0.5;

	center.x /= (6.0 * signedArea);
	center.y /= -(6.0 * signedArea);

	this->center = center;
}

void Polygon::shift(Vertex shift)
{
	this->center = this->center + shift;
	for (Vertex& v : this->vertices)
	{
		v = v + shift;
	}
}

void Polygon::print()
{
	std::cout << "polygon(";
	for (int x = 0; x < 2; x++)
	{
		for (int i = 0; i < vertices.size(); i++)
		{
			std::cout << "(" << vertices[i].x << ", " << vertices[i].y << ")";
			if (i < vertices.size() - 1)
			{
				std::cout << ",";
			}
		}
		std::cout << "\n";
	}
}