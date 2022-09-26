#include <iostream>
#include "system.h"
#include "particleSystem.h"
#include "playerSystem.h"
#include "mainPlayer.h"

using sf::Texture;
using sf::Sprite;
using sf::Event;
using sf::Keyboard;
using std::cin;
using std::cout;

string playerName;
PlayMode playmode;

void play() {
	RenderWindow* window = new RenderWindow(VideoMode(System::WindowX, System::WindowY), "IO Game!");
	window->setFramerateLimit(60);

	Texture* texture = new Texture;
	texture->loadFromFile("images/space.jpg");
	Sprite* sprite = new Sprite(*texture);

	MainPlayer *mainplayer = new MainPlayer(window, playerName);
	ParticleSystem particleSystem(window, mainplayer, 1500);
	PlayerSystem playerSystem(window, mainplayer, playmode);

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
		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		//	for (int i = 0; i < 100; i++) mainplayer->grow();
		//}

		mainplayer->move();

		particleSystem.update();

		playerSystem.draw();
		particleSystem.draw();

		window->display();
	}

	delete texture;
	delete sprite;
	delete mainplayer;
	delete window;
}

void start() {
	char x = '\0';
	
	while (x != 'y')
	{
		cout << "닉네임 입력: ";
		cin >> playerName;
		cout << "이 닉네임으로 하시겠습니까? y/아무 키 : ";
		cin >> x;
	}

	while (true)
	{
		std::cout << "호스트 하기: 1, 접속하기: 2\n입력: ";
		char input;
		cin >> input;

		switch (input)
		{
		case '1':
			playmode = PlayMode::host;
			return ;
		case '2':
			playmode = PlayMode::client;
			return ;
		default:
			std::cout << "잘못된 입력입니다.\n";
		}
	}
}

int main()
{
	start();
	play();
	return 0;
}