#include "ENGINE.h"

void Engine::run(std::vector<Polygon> polygons)
{
	for (int y = 0; y < polygons.size() - 1; y++)
	{
		for (int x = 0; x < polygons.size(); x++)
		{
			HST(polygons[y], polygons[x]);
		}
	}
}

void Engine::HST(Polygon& p1, Polygon& p2)
{

}

void Engine::processCollision(Polygon& p1, Polygon& p2)
{

}
