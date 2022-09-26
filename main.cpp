#include <iostream>
#include "system.h"
#include "particleSystem.h"
#include "playerSystem.h"
#include "mainPlayer.h"
#include "client.h"

using sf::Texture;
using sf::Sprite;
using sf::Event;
using sf::Keyboard;

void play(PlayMode playmode) {
	RenderWindow* window = new RenderWindow(VideoMode(System::WindowX, System::WindowY), "IO Game!");
	window->setFramerateLimit(60);

	Texture* texture = new Texture;
	texture->loadFromFile("images/space.jpg");
	Sprite* sprite = new Sprite(*texture);

	ParticleSystem particleSystem(window, 1500);
	PlayerSystem playerSystem(window, playmode);

	Event event;

	while (window->isOpen())
	{
		System::printFPS();
		while (window->pollEvent(event))
		{
			if (event.type == Event::Closed)
				window->close();
		}
		window->clear();
		window->draw(*sprite);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && System::buttonCoolDown()) {
			System::changeScreenMode(window);
		}

		
		playerSystem.update();
		particleSystem.update(&playerSystem);
		playerSystem.draw();
		particleSystem.draw();

		window->display();
	}

	delete texture;
	delete sprite;
	delete window;
}

PlayMode start() {
	
again:
	std::cout << "호스트 하기: 1, 접속하기: 2\n입력: ";
	char input;
	std::cin >> input;

	switch (input)
	{
	case '1':
		return PlayMode::host;
	case '2':
		return PlayMode::client;
	default:
		std::cout << "잘못된 입력입니다.\n";
		goto again;
		break;
	}
}

int main()
{
	play(start());
	return 0;
}