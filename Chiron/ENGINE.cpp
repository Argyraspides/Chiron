#include "ENGINE.h"

void Engine::run(std::vector<Polygon> &polygons)
{
	for (Polygon &p : polygons)
	{
		p.update();
		p.render();
	}

	if (polygons.size() > 1)
	{
		for (int y = 0; y < polygons.size() - 1; y++)
		{
			for (int x = 1; x < polygons.size(); x++)
			{
				if (HST(polygons[y], polygons[x]))
				{
					std::cout << "COLLISION" << "\n";
				}
				else
				{
					std::cout << "." << "\n";
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
