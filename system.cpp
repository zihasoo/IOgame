#include "system.h";
#include <SFML/System.hpp>
#include <iostream>

void System::printFPS() {
	static int lastPrintTime = 0, count = 0;
	count++;
	int curTime = clock.getElapsedTime().asMilliseconds();
	if (curTime - lastPrintTime >= 1000) {
		lastPrintTime = curTime;
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

bool System::buttonCoolDown() {
	static int lastButtonPressedTime = 0;
	int curTime = clock.getElapsedTime().asMilliseconds();
	if ((curTime - lastButtonPressedTime) > 1000) { //1s
		lastButtonPressedTime = curTime;
		return true;
	}
	return false;
}

bool System::soundCoolDown() {
	static int lastSoundPlayedTime = 0;
	int curTime = clock.getElapsedTime().asMilliseconds();
	if ((curTime - lastSoundPlayedTime) > 50) { //0.05s
		lastSoundPlayedTime = curTime;
		return true;
	}
	return false;
}

mt19937 System::gen = mt19937(time(NULL));
uni System::color = uni(0, 255);
uni System::posX = uni(0, System::MapX);
uni System::posY = uni(0, System::MapY);
Clock System::clock = Clock();
Font System::defaultFont = Font{};