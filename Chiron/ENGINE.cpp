#include "ENGINE.h"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
void Engine::run(std::vector<Polygon> &polygons)
{
	Vertex flip;

	for (Polygon &p : polygons)
	{
		p.update();
		p.render();

		if (collidesWithWall(p, flip))
		{
			/* The polygon, if it is going sufficiently fast, may not "escape" the wall line before the next
			* application loop, and hence get "stuck" in the wall as its velocity vector is flipped back and forth
			* continuously. We make sure to shift it by how fast it is going by to make sure it is no longer
			* past the wall.
			*/
			p.shift(flip * p.vel.length());
			processWallCollision(p, flip);
		}
	}

	if (polygons.size() > 1)
	{
		for (int y = 0; y < polygons.size() - 1; y++)
		{
			for (int x = 1; x < polygons.size(); x++)
			{
				if (HST(polygons[y], polygons[x]))
				{
					processCollision(polygons[y], polygons[x]);
				}
			}
		}
	}
}

bool Engine::HST(Polygon& p1, Polygon& p2)
{
	Polygon* s1 = &p1;
	Polygon* s2 = &p2;

	

	for (int s = 0; s < 2; s++)
	{
		// Second run we compare Shape 2 with 1 instead of 1 with 2
		if (s == 1)
		{
			s1 = &p2;
			s2 = &p1;
		}
		
		for (int v = 0; v < s1->vertices.size(); v++)
		{
			Vertex norm =
				(s1->vertices[v].normal(s1->vertices[(v + 1) % s1->vertices.size()]));


			float min1 =  std::numeric_limits<float>::infinity(), 
				  max1 = -std::numeric_limits<float>::infinity();

			for (int p = 0; p < s1->vertices.size(); p++)
			{
				float dotProd = s1->vertices[p].dotProduct(norm);
				min1 = std::min(min1, dotProd);
				max1 = std::max(max1, dotProd);
			}

			float min2 =  std::numeric_limits<float>::infinity(),
				  max2 = -std::numeric_limits<float>::infinity();

			for (int p = 0; p < s2->vertices.size(); p++)
			{
				float dotProd = s2->vertices[p].dotProduct(norm);
				min2 = std::min(min2, dotProd);
				max2 = std::max(max2, dotProd);
			}

			if (!(max1 >= min1 && max1 >= min2))
			{
				return false;
			}
		}
	}

	return true;
}

void Engine::processCollision(Polygon& p1, Polygon& p2)
{
	Vertex prev = { p1.vel.x, p1.vel.y };

	p1.vel.x = (((p2.mass * p2.vel.x + (p1.vel.x * p1.mass)) - p2.mass * p1.vel.x + p2.mass * p2.vel.x) /
		(p2.mass + p1.mass)) * p1.elasticity;

	p1.vel.y = (((p2.mass * p2.vel.y + (p1.vel.y * p1.mass)) - p2.mass * p1.vel.y + p2.mass * p2.vel.y) /
		(p2.mass + p1.mass)) * p1.elasticity;

	p2.vel.x = p1.vel.x + prev.x - p2.vel.x;
	p2.vel.y = p1.vel.y + prev.y - p2.vel.y;
}

void Engine::processWallCollision(Polygon& p1, Vertex &flip)
{
	p1.vel = p1.vel * flip;
}

bool Engine::collidesWithWall(Polygon& p1, Vertex &flip)
{
	std::vector<Vertex> wallVecs =
	{
		{-1,0}, // left wall
		{0,1} , // top wall
		{1,0} , // right wall
		{0,-1}  // bottom wall
	};



	for (int i = 0; i < 4; i++)
	{
		/* 
		* Find the vertex furthest along the direction of the current wall. (1)
		* This will be the point closest to the wall we are currently checking. 
		* We then just need to know if this point is past the wall. (2)
		*/

		// (1)
		float maxDotProd = -std::numeric_limits<float>::infinity();
		int furthestPointIndex = 0;

		for (int p = 0; p < p1.vertices.size(); p++)
		{
			float dotProd = p1.vertices[p].dotProduct(wallVecs[i]);
			if (dotProd > maxDotProd)
			{
				maxDotProd = dotProd;
				furthestPointIndex = p;
			}
		}

		// (2)
		switch (i) 
		{
			case 0:
				if (p1.vertices[furthestPointIndex].x <= 0)
				{
					flip = { -1, 1 };
					return true;
				}
				break;

			case 1:
				if (p1.vertices[furthestPointIndex].y >= SCREEN_HEIGHT)
				{
					flip = { 1, -1 };
					return true;
				}
				break;

			case 2:
				if (p1.vertices[furthestPointIndex].x >= SCREEN_WIDTH)
				{
					flip = { -1, 1 };
					return true;
				}
				break;

			case 3:
				if (p1.vertices[furthestPointIndex].y <= 0)
				{
					flip = { 1, -1 };
					return true;
				}
				break;
		}
	}
	return false;
}

void Engine::printPolygonCoords(Polygon& p1)
{
	for (int i = 0; i < p1.vertices.size(); i++) 
	{
		std::cout << "(" << p1.vertices[i].x << ", " << p1.vertices[i].y << "), ";
	}
	std::cout << "\n";
}