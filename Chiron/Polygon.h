#pragma once
#include <vector>
#include "Vertex.h"
#include <SFML\Graphics.hpp>
class Polygon
{
public:
	 
	Vertex center = { 0,0,0 };
	Vertex vel = { 0,0,0 };
	Vertex shiftVec;
	


	std::vector<Vertex> vertices;

	sf::ConvexShape renderedShape;

	float ang_vel = 0;
	float rot_inertia = 0;
	float mass = 1;
	float elasticity = 1;
	float energy = 0;
	float og_energy = 0;

	void update();
	void rotate();
	void rotate_deg(float degrees);
	void render();
	void shift(Vertex shift);
	void print();

	void findCentroid(std::vector<Vertex> vertices);

	void getRotationalInertia();

	Polygon(float xInit, float yInit, float mass, std::vector<Vertex> &vertices);

};
