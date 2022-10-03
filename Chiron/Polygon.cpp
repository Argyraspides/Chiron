#include "Polygon.h"

Polygon::Polygon(float &xInit, float &yInit, float &mass)
{
	this->center = { xInit, yInit };
	this->mass = mass;
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
		v = v - center;

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
	renderedShape.move(vel.x, vel.y);
	/*
	* SFML's .rotate() takes in degrees, so we make the conversion here.
	* Degrees = Radians * (180/PI)
	*/ 
	renderedShape.rotate(ang_vel * (180.0f / 3.14159265359));
}

