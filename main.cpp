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
using stringPlayModePair = std::pair<string, PlayMode>;

RenderWindow window = RenderWindow(VideoMode(System::WindowX, System::WindowY), "IO Game!");
Event event;

void play(stringPlayModePair init) {
	window.setFramerateLimit(60);

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

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && System::buttonCoolDown()) {
			System::changeScreenMode(window);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			for (int i = 0; i < 100; i++) mainplayer.grow();
		}

		mainplayer.move();

		particleSystem.update();

		playerSystem.draw();
		particleSystem.draw();

		window.display();
	}
}

stringPlayModePair start() {
	char x = '\0';
	stringPlayModePair init;
	while (x != 'y')
	{
		cout << "�г��� �Է�: ";
		cin >> init.first;
		cout << "�� �г������� �Ͻðڽ��ϱ�? y/�ƹ� Ű : ";
		cin >> x;
	}

again:

	std::cout << "ȣ��Ʈ �ϱ�: 1, �����ϱ�: 2\n�Է�: ";
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
		std::cout << "�߸��� �Է��Դϴ�.\n";
		goto again;
	}
	
	return init;
}

int main()
{
	play(start());
	return 0;
}