#pragma once
#include <random>
#include <SFML/Graphics.hpp>

using sf::Color;
using sf::Vector2f;
using std::mt19937;
using std::random_device;
using uni = std::uniform_int_distribution<int>;

class System {
private:
	static mt19937 gen;
	static uni color;
	static uni posX;
	static uni posY;
	static Color Colors[8];
public:
	static const int WindowX = 1500;
	static const int WindowY = 800;

	static const int MapX = 6000;
	static const int MapY = 3200;

	static void printFPS();
	static Color getRandomColor();
	static Vector2f getRandomPos();
};