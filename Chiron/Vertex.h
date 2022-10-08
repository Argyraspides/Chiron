#pragma once
#include "Line.h"
typedef struct Vertex {

	float x = 0;
	float y = 0;
	float z = 0;

	Vertex operator+(const Vertex& a)
	{
		return 
		{ 
			this->x + a.x, 
			this->y + a.y, 
			this->z + a.z 
		};
	};

	Vertex operator-(const Vertex& a)
	{
		return 
		{ 
			this->x - a.x, 
			this->y - a.y, 
			this->z - a.z 
		};
	};

	Vertex operator*(const float& a)
	{
		return 
		{ 
			this->x * a, 
			this->y * a, 
			this->z * a 
		};
	};

	Vertex operator/(const float& a)
	{
		return 
		{ 
			this->x / a, 
			this->y / a, 
			this->z / a 
		};
	};
	
	Vertex operator*(const Vertex& v)
	{
		return
		{
			this->x * v.x,
			this->y * v.y,
			this->z * v.z
		};
	}

	void   operator=(const Vertex& v)
	{
		this->x = v.x;
		this->y = v.y;
	};

	void normalize()
	{
		*this =

			*this
			/
			sqrt
			(
				this->x * this->x
				+
				this->y * this->y
				+
				this->z * this->z
			);
	};

	void print()
	{
		std::cout << "(" << this->x << ", " << this->y << ")" << "\n";
	}

	Vertex crossProduct(const Vertex& v2)
	{
		return
		{
			v2.y * this->z - v2.z * this->y,
			v2.z * this->x - v2.x * this->z,
			v2.x * this->y - v2.y * this->x
		};
	};

	Vertex normal(const Vertex& v2)
	{
		float dx = this->x - v2.x;
		float dy = this->y - v2.y;
		return { -dy, dx };
	}

	Vertex getIntersection(Line& l1, Line& l2)
	{
		/*

		y = m1x + c1
	  - y = m2x + c2
	 -----------------------------
		0 = m1x - m2x + c1 - c2
		0 = x(m1 - m2) + c1 - c2
		x = (-c1 + c2) / (m1 - m2)
		y = m1x + c1 or m2x + c2

		*/

		float x = (-l1.c + l2.c) / (l1.m - l2.m);
		float y = l1.m * x + l1.c;

		return { x, y };

	}

	float dotProduct(const Vertex v2)
	{
		return
		{
			v2.x * this->x
			+
			v2.y * this->y
			+
			v2.z * this->z
		};
	}

	float length()
	{
		return sqrt
		(
			this->x * this->x 
			+ 
			this->y * this->y 
			+ 
			this->z * this->z
		);
	};

	bool withinPolygon(std::vector<Vertex>& vertices)
	{
		std::vector<Line> lineEqs;
		std::vector<std::vector<Vertex>> lineEqsPts;
		lineEqs.reserve(vertices.size());
		lineEqsPts.reserve(vertices.size());

		for (int i = 0; i < vertices.size(); i++)
		{
			/*
				 We obtain the equation for the line between the vertex 'i' and 'i + 1'
				 Equation described as:
				 y = b*x + (b*x1 + y1) where b = (y2 - y1) / (x2 - x1)
				'b*x' is our 'mx' and 'b*x1 + y1' is our 'c' in 'y = mx + c'
			*/

			int wrapIndex = (i + 1) % vertices.size();
			float b = (vertices[wrapIndex].y - vertices[i].y) / (vertices[wrapIndex].x - vertices[i].x);

			// l.m = b, l.m = b*bx + y1
			Line l = { b, -b * vertices[i].x + vertices[i].y };
			lineEqs.push_back(l);

			//std::cout << "y = " << l.m << "x + " << l.c << "\n";

			if (vertices[i].x < vertices[wrapIndex].x)
			{
				std::vector<Vertex> temp = { vertices[i], vertices[wrapIndex] };
				lineEqsPts.emplace_back(temp);
			}
			else
			{
				std::vector<Vertex> temp = { vertices[wrapIndex], vertices[i] };
				lineEqsPts.emplace_back(temp);
			}
		}


		int intersectionCount = 0;
		for (int i = 0; i < lineEqs.size(); i++)
		{
			/*
				Check for any intersection between the point and the equations of the line
				point is described as:
				y = this->y;

				line is described as:
				y = mx + c

				Given it is a straight horizontal line, the y intersection will simply be 'this->y'
				The x intersection will therefore be:

				this->y = mx + c
				or (this->y - c) / m = x

				ONLY consider the intersection if it is:
				Between the bounds of the points from which the line was constructed (found in lineEqsPts)
				Less than 'this->x'
			*/

			Vertex intersection = { (this->y - lineEqs[i].c) / lineEqs[i].m, this->y };

			if (intersection.x < this->x)
			{
				if (intersection.x > lineEqsPts[i][0].x && intersection.x < lineEqsPts[i][1].x)
				{
					//std::cout << "(" << intersection.x << ", " << intersection.y << ")" << std::endl;
					intersectionCount++;
				}
			}

		}

		if (intersectionCount == 1) return true;
		return false;

	}

	bool f_withinPolygon(std::vector<Vertex>& vertices)
	{

		// Taken from: https://wrfranklin.org/Research/Short_Notes/pnpoly.html

		int i, j;
		bool c = 0;
		for (i = 0, j = vertices.size() - 1; i < vertices.size(); j = i++) {
			if (((vertices[i].y > this->y) != (vertices[j].y > this->y)) &&
				(this->x < (vertices[j].x - vertices[i].x) * (this->y - vertices[i].y) / (vertices[j].y - vertices[i].y) + vertices[i].x))
				c = !c;
		}
		return c;
	}

	Line getLineEq(Vertex& v2)
	{
		/*

		 let m = (y2 - y1) / (x2 - x1)

		 y - y1 = m(x - x1)
		 y = mx - mx1 + y1

		 'm' is our 'm' and 'lx1 + y1' is our 'c' in 'y = mx + c'

		*/

		Line l;

		l.m = (v2.y - this->y) / (v2.x - this->x);
		l.c = -l.m * this->x + this->y;

		return l;
	}


	float getDistance(Vertex& v2)
	{
		return sqrtf
		(
			(v2.x - this->x) * (v2.x - this->x)
			+ 
			(v2.y - this->y) * (v2.y - this->y)
		);
	}

};
