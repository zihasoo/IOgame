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
		cout << "�г��� �Է�: ";
		cin >> init.first;
		cout << "�� �г������� �Ͻðڽ��ϱ�? y/�ƹ� Ű : ";
		cin >> x;
	}

again:

	cout << "ȣ��Ʈ �ϱ�: 1, �����ϱ�: 2\n�Է�: ";
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
		cout << "�߸��� �Է��Դϴ�.\n";
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