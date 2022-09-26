#include "playerSystem.h"
#include <iostream>

using sf::Text;
using sf::String;
using std::cout;
using std::cin;

PlayerSystem::PlayerSystem(RenderWindow* window, PlayMode playmode) : window(window) {
	font.loadFromFile("fonts/Pretendard-Regular.ttf");
	mainPlayer = make_unique<MainPlayer>(window);

	if (playmode == PlayMode::host) {
		host = nullptr;
	}
	else {
		host = make_unique<TcpSocket>();
		do
		{
			cout << "연결 시도 중\n";
		} while (host->connect(ip, port) != sf::Socket::Done);

		cout << "-----연결 완료-----\n";
		thread t1(&PlayerSystem::receiveHost, this);
		t1.detach();
	}
}

void PlayerSystem::draw() {
	for (const auto& player : players)
	{
		window->draw(player.second);
		window->draw(Text(String(player.first), font));
	}
}

void PlayerSystem::update() {
	mainPlayer->move();
}

void PlayerSystem::receiveHost() {
	std::string name;
	float x, y;
	float radius;
	Packet data;

	while (true)
	{
		if (host->receive(data) != sf::Socket::Done) {
			std::cout << "호스트의 접속이 종료되었습니다.\n";
			exit(1);
		}

		for (int i = 0; i < players.size(); i++)
		{
			data >> name >> x >> y >> radius;
			players[name].setWorldPos(x, y);
			players[name].setRadius(radius);
		}
	}
}