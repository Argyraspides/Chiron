#pragma once
#include "Polygon.h"
#include <cmath>

class Engine
{
public:

	Engine() = default;



	// Hyperplane separation theorem algorithm
	void HST             (Polygon& p1, Polygon& p2);
	void processCollision(Polygon& p1, Polygon& p2);

	void run(std::vector<Polygon> polygons);


};