#pragma once
#include <cmath>

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

	void   operator=(const Vertex& v)
	{
		this->x = v.x;
		this->y = v.y;
	};
	Vertex   operator*(const Vertex& v)
	{
		return
		{
			this->x * v.x,
			this->y * v.y,
			this->z * v.z
		};
	}

	float dotProduct(const Vertex& v2)
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



};
