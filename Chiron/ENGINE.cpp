#include "ENGINE.h"

void Engine::run(std::vector<Polygon> &polygons)
{
	Vertex flip;

	for (Polygon &p : polygons)
	{
		p.update();
		p.render();
		if (collidesWithWall(p, flip))
		{
			processWallCollision(p, flip);
		}
	}

	if (polygons.size() > 1)
	{
		for (int y = 0; y < polygons.size() - 1; y++)
		{
			for (int x = y + 1; x < polygons.size(); x++)
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
		// On the second run, we take the vertex projections onto the axes formed by the normals
		// of the sides on Shape 2 instead of Shape 1.
		if (s == 1)
		{
			s1 = &p2;
			s2 = &p1;
		}
		
		/*
		* Assuming you know what the Separation-Axis Theorem/Hyperplane Separation Theorem (SAT/HST) is:
		* 
		* For the current shape we are taking the side normals of, 
		* We compute the dot product of each shapes vertices with this normal.
		* 
		* Let |X| = Length of vector X, and '.' be the dot product.
		* 
		* For vectors A and B, (A.B) / |B| would give us the length of the vector A projected onto B.
		* 
		* We don't need to compute the extra division |B| as this would just scale down all the 
		* projection points proportionally, and doesn't help us in any way determining if there are overlapping
		* points.
		* 
		* 
		* E.g (let Px be a point of projection)
		* 
		* 0 ------ 1 ------ 2 ------ 3 ------ 4 ------ 5 ------ 6 ------ 7 ------ 8 ------ 9 --> (Axis of Projection)
		*		   P1		P2				  P3								  P4	
		* 
		* ^ Suppose |B| from the equation above was 2. Dividing these points by 2 gives us:
		* 
		* 
		*  0 ------ 1 ------ 2 ------ 3 ------ 4 ------ 5 ------ 6 ------ 7 ------ 8 ------ 9 --> (Axis of Projection)
		*      P1   P2		 P3				   P4	
		* 
		* Our original goal was to see if the line created by P1 and P2 overlaps with the line created by P3 and P4.
		* 
		* Dividing the values of these points doesn't help us at all in this regard, so we can skip it and simply take the dot product.
		* 
		* The precise points of projection could be determined by using sin() and cos() whilst considering
		* a rotated coordinate system. This is computationally expensive and hence wasn't used.
		*/

		for (int v = 0; v < s1->vertices.size(); v++)
		{
			Vertex projectionAxis =
				(s1->vertices[v].normal(s1->vertices[(v + 1) % s1->vertices.size()]));


			float min1 =  std::numeric_limits<float>::infinity(), 
				  max1 = -std::numeric_limits<float>::infinity();
		
			for (int p = 0; p < s1->vertices.size(); p++)
			{
				float dotProd = s1->vertices[p].dotProduct(projectionAxis);
				min1 = std::min(min1, dotProd);
				max1 = std::max(max1, dotProd);
			}

			float min2 =  std::numeric_limits<float>::infinity(),
				  max2 = -std::numeric_limits<float>::infinity();

			for (int p = 0; p < s2->vertices.size(); p++)
			{
				float dotProd = s2->vertices[p].dotProduct(projectionAxis);
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

	for (int i = 0; i < wallVecs.size(); i++)
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
			/*
			*  (3)
			*  The polygon, if it is going sufficiently fast, may not "escape" the wall line before the next
			*  application loop, and hence get "stuck" in the wall as its velocity vector is flipped back and forth
			*  continuously. We make sure to shift by how much it has penetrated the wall to make sure it is no longer
			*  past it.
			*
			*  We don't want to shift it left or right when we hit the top or bottom walls
			*  nor do we want to shift if up or down if we hit the side walls. We set
			*  Whichever component of flip is '1' (meaning that that component was the one we
			*  DIDN'T invert) will be set to 0.
			*/


		// (2)
		switch (i) 
		{
			case 0:
				if (p1.vertices[furthestPointIndex].x < 0)
				{
					flip = { -1, 1 };
					p1.shift({-p1.vertices[furthestPointIndex].x, 0}); // (3)
					return true;
				}
				break;

			case 1:
				if (p1.vertices[furthestPointIndex].y > SCREEN_HEIGHT)
				{
					flip = { 1, -1 };
					p1.shift({0 , SCREEN_HEIGHT - p1.vertices[furthestPointIndex].y }); // (3)
					return true;
				}
				break;

			case 2:
				if (p1.vertices[furthestPointIndex].x > SCREEN_WIDTH)
				{
					flip = { -1, 1};
					p1.shift({ SCREEN_WIDTH - p1.vertices[furthestPointIndex].x, 0 }); // (3)
					return true;
				}
				break;

			case 3:
				if (p1.vertices[furthestPointIndex].y < 0)
				{
					flip = { 1, -1 };
					p1.shift({0, -p1.vertices[furthestPointIndex].y }); // (3)
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
		std::cout << "(" << p1.vertices[i].x << ", " << -p1.vertices[i].y << ")";
		if (i < p1.vertices.size() - 1)
		{
			std::cout << ",";
		}
	}
	std::cout << "\n";
}