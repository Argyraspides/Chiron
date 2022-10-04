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
/*
POLYGON::ROATATE()

		 Formula for a point's coordinates after rotation around the center:

		 x * cos(t) - y * sin(t),
		 y * cos(t) + x * sin(t)

		 Once we move the shape back to the origin via shape.position -= shape.position,
		 then each vertex will be at a location vertex -= shape.position.
		 we can use that formula above to rotate each point around the origin,
		 then move all the points back with vertex += shape.position

*/

void Polygon::rotate()
{
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
	renderedShape.rotate(ang_vel * (180.0f / 3.14159265359));
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
}