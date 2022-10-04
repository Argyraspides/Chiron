#pragma once
#include "Polygon.h"
#include <iostream>
#include <cmath>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

class Engine
{
public:

	Engine() = default;

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

	/* 
	 * Tells us if the polygon 'p1' has collided with one of the walls.
     * Sets the 'flip' vertex to be whatever velocity component needs
	 * to be flipped (e.g. when the polygon hits the top or bottom wall, the y velocity is flipped)
	*/
	bool collidesWithWall      (Polygon& p1, Vertex &flip);
	// Flips the appropriate velocity vector.
	void processWallCollision  (Polygon& p1, Vertex &flip);

	void printPolygonCoords(Polygon& p);
	void run(std::vector<Polygon> &polygons);


};