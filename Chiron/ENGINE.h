#pragma once
#include "Polygon.h"
#include <iostream>
#include <cmath>

#define SCREEN_WIDTH 1366
#define SCREEN_HEIGHT 768
#define TEMP_INERTIA 45000

class Engine
{
public:
	Engine() = default;

	float collisionCount = 0;

	std::vector<Vertex> wallVecs =
	{
		{-1,0}, // left wall
		{0,1} , // top wall
		{1,0} , // right wall
		{0,-1}  // bottom wall
	};

	// Hyperplane separation theorem algorithm
	bool HST(Polygon& p1, Polygon& p2);
	// Processing Translational-only collision
	void processCollision      (Polygon& p1, Polygon& p2);
	void processCollision_ang(Polygon& p1, Polygon& p2, Vertex& collisionPoint, Vertex& n);

	// Gilbert-Johnathan-Keerthi Algorithm
	bool GJK(Polygon& a1, Polygon& a2);
	// Retrieves the support point for the GJK algorithm
	Vertex support(Polygon& a1, Polygon& a2, Vertex& vector);
	// Checks if the current triangle, or "simplex" contains the origin.
	bool checkTriangle(std::vector<Vertex>& triangle, Vertex& direction);

	/* 
	 * Tells us if the polygon 'p1' has collided with one of the walls.
     * Sets the 'flip' vertex to be whatever velocity component needs
	 * to be flipped (e.g. when the polygon hits the top or bottom wall, the y velocity is flipped)
	*/
	bool collidesWithWall      (Polygon& p1, Vertex &flip);
	// Flips the appropriate velocity vector.
	void processWallCollision  (Polygon& p1, Vertex &flip);

	// Updates and renders all shapes, compares pairs to evaluate collision 
	void run(std::vector<Polygon> &polygons);

	// Obtains the exact point of collision between two polygons once they have collided
	Vertex getCollisionPoint(Polygon& p1, Polygon& p2, bool &origin);

	/* Gets the collision normal of the two polygons (the collision normal is defined as
	*  the normal to the edge of the polygon on which the collision occured). 
	* 
	*  If there is an edge-edge collision, taking the normal of either edge will work.
	* 
	* "origin" is an identifier for which polygon had its vertex-edge collision.
	*  E.g. if Polygon1's vertex collided with Polygon2's edge, then "origin" would be false.
	*  If Polygon2's vertex collided with Polygon1's edge, then "origin" would be true.
	*  If there is an edge-edge collision, it doesn't matter which one we take (hence it is 
	*  initially set to false).
	*/
	Vertex getCollisionNormal(Polygon& p1, Polygon& p2, Vertex& collisionPoint, bool &origin);

	// Separates the shapes by the appropriate amount, such that they are no longer overlapping
	void separatePolygons(Polygon& p1, Polygon& p2, Vertex& collisionPoint, bool& origin);

};