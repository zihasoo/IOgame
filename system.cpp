#include "system.h";
#include <SFML/System.hpp>
#include <iostream>

void System::printFPS() {
	static sf::Clock clock;
	static int count = 0;

	count++;
	if (clock.getElapsedTime().asMilliseconds() >= 1000) {
		clock.restart();
		std::cout << "FPS: " << count << '\n';
		count = 0;
	}
}

Color System::getRandomColor() {
	return Color(color(gen), color(gen), color(gen));
}

Vector2f System::getRandomPos() {
	return Vector2f(posX(gen), posY(gen));
}

mt19937 System::gen = mt19937(time(NULL));
uni System::color = uni(0, 255);
uni System::posX = uni(0, System::MapX);
uni System::posY = uni(0, System::MapY);