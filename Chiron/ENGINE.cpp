#include "ENGINE.h"


void   Engine::run(std::vector<Polygon>& polygons)
{
	Vertex flip;

	// Update, render, and check for any wall collisions
	for (Polygon& p : polygons)
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

				if (GJK(polygons[y], polygons[x]))
				{
					// Determines if p1's vertex collided with p2's edge, or vice versa.
					// If there was an edge-edge collision, it doesn't matter whether this is
					// true or false.
					bool origin = false;

					Vertex collisionPoint = getCollisionPoint(polygons[y], polygons[x], origin);
					Vertex n = getCollisionNormal(polygons[y], polygons[x], collisionPoint, origin);

					processCollision_ang(polygons[y], polygons[x], collisionPoint, n);
					separatePolygons(polygons[y], polygons[x], collisionPoint, origin);
				}
			}
		}
	}
}

Vertex Engine::getCollisionPoint(Polygon& p1, Polygon& p2, bool& origin)
{
	// Points of polygon 1 that are inside polygon 2, and vice versa, respectively.
	std::vector<Vertex> lyingPoints1, lyingPoints2;
	lyingPoints1.reserve(1); lyingPoints2.reserve(1);

	Polygon* p1Ptr = &p1;
	Polygon* p2Ptr = &p2;

	for (int p = 0; p < 2; p++)
	{
		// Check for polygon 2's points inside polygon 1's the second iteration.
		if (p == 1)
		{
			p1Ptr = &p2;
			p2Ptr = &p1;
		}

		for (int i = 0; i < p1Ptr->vertices.size(); i++)
		{
			if (p1Ptr->vertices[i].f_withinPolygon(p2Ptr->vertices))
			{
				if (p == 0)
				{
					lyingPoints1.push_back(p1Ptr->vertices[i]);
				}
				else
				{
					lyingPoints2.push_back(p1Ptr->vertices[i]);
				}
			}
		}

	}

	if (lyingPoints1.size() == 1 && lyingPoints2.size() == 0)
	{
		// This means that Polygon1 has collided with Polygon2's edge
		origin = false;
		return lyingPoints1[0];
	}
	else if (lyingPoints1.size() == 0 && lyingPoints2.size() == 1)
	{
		// This means that Polygon2 has collided with Polygon1's edge.
		origin = true;
		return lyingPoints2[0];
	}
	else if (lyingPoints1.size() == 1 && lyingPoints2.size() == 1)
	{
		/*
		This means that there was an edge-edge collision, so we simply take
		the average of the two points. Origin can either be true or false.
		It has already been initialized to false in ENGINE.run().
		*/
		return (lyingPoints1[0] + lyingPoints2[0]) * 0.5f;
	}
	else if (lyingPoints1.size() == 2 && lyingPoints2.size() == 0)
	{
		// This means that the edge of Polygon2 was wide enough to completely contain the edge of
		// Polygon1
		return (lyingPoints1[0] + lyingPoints1[1]) * 0.5f;
	}
	else if (lyingPoints1.size() == 0 && lyingPoints2.size() == 2)
	{
		// This means that the edge of Polygon1 was wide enough to completely contain the edge of
		// Polygon2
		return (lyingPoints2[0] + lyingPoints2[1]) * 0.5f;
	}

}

Vertex Engine::getCollisionNormal(Polygon& p1, Polygon& p2, Vertex& collisionPoint, bool& origin)
{
	/*
	 Origin is the polygon that had its point inside the other.
	 E.g. if origin is false, then Polygon1 had one of its vertices collide with Polygon2's
	 edge, and vice versa. If there was an edge-edge collision, we can take the normal of either
	 edge so it doesn't matter what origin is (it has already been initialized to false).
	*/

	Polygon* p1Ptr = (!origin) ? &p1 : &p2;
	Polygon* p2Ptr = (!origin) ? &p2 : &p1;

	// We check which of the edges of *p1Ptr are closest to the collisionPoint.
	float dist = std::numeric_limits<float>::infinity();

	// Indecies of the vertices that make the edge which is closest to the collision point.
	int vertexIndecies[2];

	for (int i = 0; i < p2Ptr->vertices.size(); i++)
	{
		int wrapIndex = (i + 1) % p2Ptr->vertices.size();

		// Line equation for one of the sides of the polygon
		Line l = p2Ptr->vertices[i].getLineEq(p2Ptr->vertices[wrapIndex]);
		// Line equation normal to 'l' that also passes through the collision point.
		Line l_norm = l;
		// Transforms the gradient to be normal to 'l'
		l_norm.m = -(1 / l_norm.m);
		// Changes intercept to pass through the collisionPoint.
		l_norm.c = collisionPoint.y - l_norm.m * collisionPoint.x;

		Vertex intersection = p2Ptr->vertices[i].getIntersection(l, l_norm);

		float currentDist = collisionPoint.getDistance(intersection);

		if (currentDist < dist)
		{
			dist = currentDist;
			vertexIndecies[0] = i;
			vertexIndecies[1] = wrapIndex;
		}

	}

	return p2Ptr->vertices[vertexIndecies[0]].normal(p2Ptr->vertices[vertexIndecies[1]]);

}

bool   Engine::HST(Polygon& p1, Polygon& p2)
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
			// The axis on which we want to project is normal to the line formed by the points
			// s1->vertices[v] and s1->vertices[v + 1] where v + 1 wraps around to 0 on the final vertex
			int wrapIndex = (v + 1) % s1->vertices.size();
			Vertex projectionAxis =
				(s1->vertices[v].normal(s1->vertices[wrapIndex]));
			projectionAxis.normalize();

			float min1 = std::numeric_limits<float>::infinity(),
				max1 = -std::numeric_limits<float>::infinity();

			// min1 and 2, max1 and 2 are just the minimum and maximum vertex projections onto the axis.
			// for Polygon1 and 2 respectively.
			for (int p = 0; p < s1->vertices.size(); p++)
			{
				float dotProd = s1->vertices[p].dotProduct(projectionAxis);
				min1 = std::min(min1, dotProd);
				max1 = std::max(max1, dotProd);
			}

			float min2 = std::numeric_limits<float>::infinity(),
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

bool Engine::GJK(Polygon& p1, Polygon& p2)
{
	Vertex collisionPoint;
	Vertex directionVector = { 1,1 };

	std::vector<Vertex> triangle;

	triangle.push_back(support(p1, p2, directionVector));

	directionVector = { 0 - triangle.at(0).x, 0 - triangle.at(0).y };
	directionVector.normalize();


	while (true)
	{
		Vertex nextSupportPoint = support(p1, p2, directionVector);
		if (directionVector.dotProduct(nextSupportPoint) < 0) return false;

		triangle.push_back(nextSupportPoint);

		if (checkTriangle(triangle, directionVector))
		{
			return true;
		}
	}
}

Vertex Engine::support(Polygon& a1, Polygon& a2, Vertex& vector)
{

	float prevMax = -std::numeric_limits<float>::infinity();
	Vertex p1 = { 0,0 }; Vertex p2 = { 0,0 };

	for (int c = 0; c < a1.vertices.size(); c++)
	{
		float dotProd = a1.vertices[c].x * vector.x + -a1.vertices[c].y * vector.y;
		if (dotProd > prevMax)
		{
			prevMax = dotProd;
			p1 = a1.vertices[c];
		}
	}

	prevMax = -std::numeric_limits<float>::infinity();

	for (int c = 0; c < a2.vertices.size(); c++)
	{
		float dotProd = a2.vertices[c].x * -vector.x + -a2.vertices[c].y * -vector.y;
		if (dotProd > prevMax)
		{
			prevMax = dotProd;
			p2 = a2.vertices[c];
		}
	}


	return { p1.x - p2.x, -p1.y - -p2.y };
}

bool Engine::checkTriangle(std::vector<Vertex>& triangle, Vertex& direction)
{

	if (triangle.size() == 2)
	{
		Vertex AB = { triangle[0].x - triangle[1].x, triangle[0].y - triangle[1].y, 1 };
		Vertex AO = { 0 - triangle[1].x, 0 - triangle[1].y, 1 };

		Vertex toOrigin = AB.crossProduct(AO).crossProduct(AB);
		toOrigin.normalize();

		direction = toOrigin;
		return false;
	}

	Vertex A_originLine = { 0 - triangle[2].x , 0 - triangle[2].y };
	Vertex B_originLine = { 0 - triangle[1].x, 0 - triangle[1].y };
	A_originLine.normalize();

	Vertex AB = { triangle[1].x - triangle[2].x, triangle[1].y - triangle[2].y };
	Vertex AC = { triangle[0].x - triangle[2].x, triangle[0].y - triangle[2].y };
	Vertex BC = { triangle[0].x - triangle[1].x, triangle[0].y - triangle[1].y };

	Vertex perpendicular1 = (AC.crossProduct(AB)).crossProduct(AB); // (AC X AB) X AB
	perpendicular1.normalize();

	Vertex perpendicular2 = (AB.crossProduct(AC)).crossProduct(AC); // (AB X AC) X AC
	perpendicular2.normalize();

	if (perpendicular1.dotProduct(A_originLine) > 0)
	{
		triangle.erase(triangle.begin());
		direction = perpendicular1;
		return false;
	}
	else if (perpendicular2.dotProduct(A_originLine) > 0)
	{
		triangle.erase(triangle.begin() + 1);
		direction = perpendicular2;
		return false;
	}

	return true;

}

void   Engine::processCollision(Polygon& p1, Polygon& p2)
{
	Vertex prev = { p1.vel.x, p1.vel.y };

	p1.vel.x = (((p2.mass * p2.vel.x + (p1.vel.x * p1.mass)) - p2.mass * p1.vel.x + p2.mass * p2.vel.x) /
		(p2.mass + p1.mass)) * p1.elasticity;

	p1.vel.y = (((p2.mass * p2.vel.y + (p1.vel.y * p1.mass)) - p2.mass * p1.vel.y + p2.mass * p2.vel.y) /
		(p2.mass + p1.mass)) * p1.elasticity;

	p2.vel.x = p1.vel.x + prev.x - p2.vel.x;
	p2.vel.y = p1.vel.y + prev.y - p2.vel.y;
}

void   Engine::processCollision_ang(Polygon& p1, Polygon& p2, Vertex& collisionPoint, Vertex& n)
{

	float e = 0.9;
	float j;
	Vertex v_ap, v_bp, r_ap, r_bp;

	// Vector pointing from the center of each shape to the point of collision
	r_ap = collisionPoint - p1.center;
	r_bp = collisionPoint - p2.center;

	// v_ap represents the velocity of point p on Polygon A, in this case Polygon1
	// and same thing with v_bp
	Vertex temp = { p1.ang_vel * r_ap.y, p1.ang_vel * r_ap.x };
	v_bp = p1.vel + temp;

	temp = { p2.ang_vel * r_bp.y, p2.ang_vel * r_bp.x };
	v_ap = p2.vel + temp;

	// We now take the normals to the "collision arms"
	r_ap = { -r_ap.y, r_ap.x };
	r_bp = { -r_bp.y, r_bp.x };

	// Impulse formula for rotational collisions
	j =
		(

			n.dotProduct((v_bp - v_ap) * -(1.0 + e)))
		/

		(

			(n.dotProduct(n) * (1.0 / p1.mass + 1.0 / p2.mass))
			+
			pow((r_ap.dotProduct(n)), 2) / TEMP_INERTIA
			+
			pow((r_bp.dotProduct(n)), 2) / TEMP_INERTIA

			);

	// Final collision response.

	p1.vel = p1.vel + n * ((j) / p1.mass);
	p2.vel = p2.vel + n * ((-j) / p2.mass);

	p1.ang_vel = p1.ang_vel + r_ap.dotProduct((n * ((j) / TEMP_INERTIA)));
	p2.ang_vel = p2.ang_vel + r_bp.dotProduct((n * ((-j) / TEMP_INERTIA)));
}

void   Engine::separatePolygons(Polygon& p1, Polygon& p2, Vertex& collisionPoint, bool& origin)
{

	Polygon* s1 = &p1;
	Polygon* s2 = &p2;

	for (int p = 0; p < 2; p++)
	{
		if (p == 1)
		{
			s1 = &p2;
			s2 = &p1;
		}

		for (int i = 0; i < s1->vertices.size(); i++)
		{
			// Take the first shape, and find the equation of the line from its center to 
			// each of its vertices.
			Line l1 = s1->vertices[i].getLineEq(s1->center);

			// We also need to find the bounds of this line, i.e. the line segment of each shapes sides.
			float boundsX[2];

			float boundsY[2] =
			{
				std::min(s1->vertices[i].y, s1->center.y),
				std::max(s1->vertices[i].y, s1->center.y)
			};

			// Then, for all the edges on the other shape, we test for an intersection with line 'l'.

			for (int j = 0; j < s2->vertices.size(); j++)
			{
				// We make sure to "wrap" around to the first point on our last iteration
				int wrapIndex = (j + 1) % s2->vertices.size();

				// And find the equation of the line for each adjacent vertex pair.
				Line l2 = s2->vertices[j].getLineEq(s2->vertices[wrapIndex]);
				Vertex intersection = s2->vertices[0].getIntersection(l1, l2);

				boundsX[0] = std::min(s2->vertices[j].x, s2->vertices[wrapIndex].x);
				boundsX[1] = std::max(s2->vertices[j].x, s2->vertices[wrapIndex].x);

				bool withinScreen =
					(intersection.x > 0 && intersection.x < 1920)
					&&
					(intersection.y > 0 && intersection.y < 1080);

				bool withinBounds =
					(intersection.x < boundsX[1] && intersection.x > boundsX[0])
					&&
					(intersection.y < boundsY[1] && intersection.y > boundsY[0]);


				if (withinBounds && withinScreen)
				{
					/*
					
					If we have a vertex-edge collision, and we also introduce rotation, the amount we need to "resolve"
					the collision by isn't just separating them by the maximum penetration depth. This is because on the next
					frame, it is possible that the shape will penetrate the other *again*, even though they are supposed
					to be separated completely. Therefore we need to scale this maximum penetration depth by 'x', where
					'x' is how much of the velocity of the collision point 'p' is in the direction of the maximum
					penetration depth. This ensures on the next frame that the shapes don't overlap a second time.
					
					*/

					// The initial amount we wish to separate the shapes by; which is the maximum
					// penetration depth.
					Vertex shiftVertex = intersection - s1->vertices[i];

					// The "collision arm", the vector which points from the center of s1 to the collision point. 
					Vertex r_ap = collisionPoint - s1->center;
					// The velocity of point p on s1.
					Vertex v_p = { p1.ang_vel * r_ap.y, p1.ang_vel * r_ap.x };
					// We normalize v_p and shiftVertex as we only want to find the absolute % amount of how much v_p is in the 
					// direction of the shiftVertex
					// % of v_p in the direction of the normalized shiftVertex.
					float vel_scale = v_p.normalized().dotProduct(shiftVertex.normalized());
					float vel_length = v_p.length();

					if (vel_scale < 0) vel_scale *= -1;
					if (vel_scale == 0) vel_scale += 1;
					float scaleFactor_m = vel_scale * vel_length;
					scaleFactor_m = (scaleFactor_m < 1) ? (scaleFactor_m + 1) : (scaleFactor_m);
					s1->shift(shiftVertex * scaleFactor_m);
					return;
				}

			}

		}
	}
}

void   Engine::processWallCollision(Polygon& p1, Vertex& flip)
{
	p1.vel = p1.vel * flip;
}

bool   Engine::collidesWithWall(Polygon& p1, Vertex& flip)
{
	// TODO: Optimization step: define all shapes with the largest possible "bounding" box. If this box isn't 
	// colliding with the edge, then just return false.
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
				p1.shift({ -p1.vertices[furthestPointIndex].x, 0 }); // (3)
				return true;
			}
			break;

		case 1:
			if (p1.vertices[furthestPointIndex].y > SCREEN_HEIGHT)
			{
				flip = { 1, -1 };
				p1.shift({ 0 , SCREEN_HEIGHT - p1.vertices[furthestPointIndex].y }); // (3)
				return true;
			}
			break;

		case 2:
			if (p1.vertices[furthestPointIndex].x > SCREEN_WIDTH)
			{
				flip = { -1, 1 };
				p1.shift({ SCREEN_WIDTH - p1.vertices[furthestPointIndex].x, 0 }); // (3)
				return true;
			}
			break;

		case 3:
			if (p1.vertices[furthestPointIndex].y < 0)
			{
				flip = { 1, -1 };
				p1.shift({ 0, -p1.vertices[furthestPointIndex].y }); // (3)
				return true;
			}
			break;
		}
	}
	return false;
}



