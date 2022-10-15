#include "Polygon.h"

Polygon::Polygon(float xInit, float yInit, float mass, std::vector<Vertex> &vertices)
{
	this->center = { xInit, yInit };
	this->mass = mass;
	this->vertices = vertices;
	findCentroid(vertices);
	getRotationalInertia();

	
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

void Polygon::rotate_deg(float degrees)
{
	// see the rotate() function for an explanation.

	for (Vertex& v : vertices)
	{
		v = v + vel - center;

		float sRot = sin(degrees);
		float cRot = cos(degrees);

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

void Polygon::getRotationalInertia()
{

	// The formula for the second moment of inertia for an arbitrary polygon can be found here:
	// https://en.wikipedia.org/wiki/Second_moment_of_area 

	float j_x = 0, j_y = 0;

	for (int v = 0; v < vertices.size() - 1; v++)
	{
		int vpp = v + 1;
		// (x_i * y_i+1 - x_i+1 * y_i)
		float leftTerm =
			vertices[v].x * vertices[vpp].y - vertices[vpp].x * vertices[v].y;

		// (y_i^2 + y_i * y_i+1 + y_i+1^2)
		float rightTerm =
			pow(vertices[v].y, 2) + vertices[v].y * vertices[vpp].y + pow(vertices[vpp].y, 2);

		j_x += leftTerm + rightTerm;
		j_y += leftTerm;

		// (x_i^2 + x_i * x_i+1 + x_i+1^2)
		rightTerm = 
			pow(vertices[v].x, 2) + vertices[v].x * vertices[vpp].x + pow(vertices[vpp].x, 2);

		j_y += rightTerm;

	}

	float oneTwelfth = 1.0f / 12.0f;
	j_x *= oneTwelfth;
	j_y *= oneTwelfth;

	this->rot_inertia = (j_x + j_y) * mass;
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
			std::cout << "(" << vertices[i].x << ", " << -vertices[i].y << ")";
			if (i < vertices.size() - 1)
			{
				std::cout << ",";
			}
		}
		std::cout << "\n";
	}
}