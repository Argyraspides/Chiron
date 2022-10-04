#pragma once
#include "Polygon.h"
#include <iostream>
#include <cmath>

class Engine
{
public:

	Engine() = default;



	// Hyperplane separation theorem algorithm
	bool HST                   (Polygon& p1, Polygon& p2);
	void processCollision      (Polygon& p1, Polygon& p2);

	bool collidesWithWall      (Polygon& p1, Vertex &flip);
	void processWallCollision  (Polygon& p1, Vertex &flip);

	void printPolygonCoords(Polygon& p);
	void run(std::vector<Polygon> &polygons);


};