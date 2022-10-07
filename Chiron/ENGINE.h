#pragma once
#include "Polygon.h"
#include <iostream>
#include <cmath>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define TEMP_INERTIA 100

class Engine
{
public:

	Engine() = default;

	Vertex shiftVec;

	std::vector<Vertex> wallVecs =
	{
		{-1,0}, // left wall
		{0,1} , // top wall
		{1,0} , // right wall
		{0,-1}  // bottom wall
	};

	// Hyperplane separation theorem algorithm
	bool HST                   (Polygon& p1, Polygon& p2);
	// Processing Translational-only collision
	void processCollision      (Polygon& p1, Polygon& p2);
	void processCollision_ang(Polygon& p1, Polygon& p2, Vertex& collisionPoint, Vertex& n);

	/* 
	 * Tells us if the polygon 'p1' has collided with one of the walls.
     * Sets the 'flip' vertex to be whatever velocity component needs
	 * to be flipped (e.g. when the polygon hits the top or bottom wall, the y velocity is flipped)
	*/
	bool collidesWithWall      (Polygon& p1, Vertex &flip);
	// Flips the appropriate velocity vector.
	void processWallCollision  (Polygon& p1, Vertex &flip);

	// Prints the coordinates of the vertices of the polygon p
	void printPolygonCoords(Polygon& p);

	// Updates and renders all shapes, compares pairs to evaluate collision 
	void run(std::vector<Polygon> &polygons);

	// Obtains the exact point of collision between two polygons once they have collided
	Vertex getCollisionPoint(Polygon& p1, Polygon& p2, Vertex &n);

	bool pointInPolygon(Vertex& point, Polygon& polygon);


	// Methods for checking if there lies a point inside of a polygon
	//bool onSegment(Vertex &p, Vertex &q, Vertex &r);
	//bool doIntersect(Vertex &p1, Vertex &q1, Vertex &p2, Vertex &q2);
	//bool isInside(Polygon &polygon, int n, Vertex &p);
	//int  orientation(Vertex& p, Vertex& q, Vertex& r);
};