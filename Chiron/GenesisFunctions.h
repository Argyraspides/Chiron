#pragma once
#include <iostream>
#include <SFML\Graphics.hpp>
#include "ENGINE.h"

void runEngine();
void handleInput(sf::Event& event);
void makeShape();
std::vector<sf::CircleShape> drawingPoints;
Engine engine;
sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Chiron");
std::vector<Polygon> polygons;

