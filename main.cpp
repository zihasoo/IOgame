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
		cout << "�г��� �Է�: ";
		cin >> playerName;
		cout << "�� �г������� �Ͻðڽ��ϱ�? y/�ƹ� Ű : ";
		cin >> x;
	}

	while (true)
	{
		std::cout << "ȣ��Ʈ �ϱ�: 1, �����ϱ�: 2\n�Է�: ";
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
			std::cout << "�߸��� �Է��Դϴ�.\n";
		}
	}
}

int main()
{
	start();
	play();
	return 0;
}