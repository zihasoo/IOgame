#pragma once
#include <random>
#include <SFML/Graphics.hpp>

using sf::Color;
using sf::Vector2f;
using sf::Clock;
using sf::RenderWindow;
using sf::VideoMode;
using std::mt19937;
using std::random_device;
using uni = std::uniform_int_distribution<int>;

class System {
private:
	static mt19937 gen;
	static uni color;
	static uni posX;
	static uni posY;
	static Clock clock;
	enum class ScreenMode
	{
		FullScreen,
		FloatScreen
	};
	static ScreenMode curentScreenMode;

public:
	static const int WindowX = 720; //1440
	static const int WindowY = 405; //810

	static const int MapX = 3840;
	static const int MapY = 2160;

	static void printFPS();
	static void changeScreenMode(RenderWindow*);
	static bool buttonCoolDown();
	static bool soundCoolDown();
	static Color getRandomColor();
	static Vector2f getRandomPos();
};