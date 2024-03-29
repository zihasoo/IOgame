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
using initpr = std::pair<string, PlayMode>;

RenderWindow window = RenderWindow(VideoMode(System::WindowX, System::WindowY), "IO Game!");
Event event;

initpr start() {
	System::defaultFont.loadFromFile("fonts/Pretendard-Regular.ttf");
	window.setFramerateLimit(60);

	char x = '\0';
	initpr init;
	while (x != 'y')
	{
		cout << "닉네임 입력: ";
		cin >> init.first;
		cout << "이 닉네임으로 하시겠습니까? y/아무 키 : ";
		cin >> x;
	}

again:

	cout << "호스트 하기: 1, 접속하기: 2\n입력: ";
	char input;
	cin >> input;

	switch (input)
	{
	case '1':
		init.second = PlayMode::host;
		break;
	case '2':
		init.second = PlayMode::client;
		break;
	default:
		cout << "잘못된 입력입니다.\n";
		goto again;
	}
	
	return init;
}

int main()
{
#ifdef DEBUG
	System::defaultFont.loadFromFile("fonts/Pretendard-Regular.ttf");
	window.setFramerateLimit(60);
	initpr init = { "client",PlayMode::client };
#else
	initpr init = start();
#endif // DEBUG

	unique_ptr<Texture> texture = make_unique<Texture>();
	texture->loadFromFile("images/space.jpg");
	unique_ptr<Sprite> sprite = make_unique<Sprite>(*texture);

	MainPlayer mainplayer(window, init.first);
	ParticleSystem particleSystem(window, mainplayer, 1500);
	PlayerSystem playerSystem(window, mainplayer, init.second);

	while (window.isOpen())
	{
		//System::printFPS();
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}
		window.clear();
		window.draw(*sprite);

		mainplayer.move();

		particleSystem.update();

		playerSystem.draw();
		particleSystem.draw();

		window.display();
	}
	return 0;
}