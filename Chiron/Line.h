#pragma once
#define GRADIENT_TOLERANCE 0.01f
#include <cmath>
#include <iostream>
typedef struct Line
{
	float m = 0;
	float c = 0;
	bool intersects(Line& l)
	{
		// Does the line intersect at all?
		return (abs(this->m - l.m) <= GRADIENT_TOLERANCE);
	}
	bool intersectsInBounds(Line& l, float xMin, float xMax)
	{
		// Does this line intersect 'l' where the 'x' intersection is xMin <= x <= xMax

		if (abs(this->m - l.m) <= GRADIENT_TOLERANCE)
		{
			return false;
		}

		// x = (c2 - c1) / (m1 - m2)
		// y = l.m * x + l.c
		float xIntersection = (l.c - this->c) / (this->m - l.m);

		return (xIntersection < xMax&& xIntersection > xMin);

	}
};