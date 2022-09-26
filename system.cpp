#include "system.h";
#include <SFML/System.hpp>
#include <iostream>

void System::printFPS() {
	static Clock FPSclock;
	static int count = 0;
	count++;
	if (FPSclock.getElapsedTime().asMilliseconds() >= 1000) {
		FPSclock.restart();
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

void System::changeScreenMode(RenderWindow *window) {
	if (curentScreenMode == ScreenMode::FullScreen) {
		window->create(VideoMode(System::WindowX, System::WindowY), "IO Game!");
		window->setFramerateLimit(60);
		curentScreenMode = ScreenMode::FloatScreen;
	}
	else {
		window->create(VideoMode::getDesktopMode(), "IO Game!", sf::Style::Fullscreen);
		window->setFramerateLimit(60);
		curentScreenMode = ScreenMode::FullScreen;
	}
}

bool System::buttonCoolDown() {
	static int lastButtonPressedTime = 0, PressCool = 1000;
	int curTime = clock.getElapsedTime().asMilliseconds();
	if ((curTime - lastButtonPressedTime) > PressCool) {
		lastButtonPressedTime = curTime;
		return true;
	}
	return false;
}

mt19937 System::gen = mt19937(time(NULL));
uni System::color = uni(0, 255);
uni System::posX = uni(0, System::MapX);
uni System::posY = uni(0, System::MapY);
Clock System::clock = Clock();
System::ScreenMode System::curentScreenMode = System::ScreenMode::FloatScreen;