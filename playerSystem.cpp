#include "playerSystem.h"
#include <iostream>

using sf::Text;
using sf::String;
using sf::Clock;
using std::cout;
using std::cin;

PlayerSystem::PlayerSystem(RenderWindow* window, MainPlayer* player, PlayMode playmode) : window(window), mainPlayer(player) {
	font.loadFromFile("fonts/Pretendard-Regular.ttf");

	if (playmode == PlayMode::host) {
		
	}
	else {

		do
		{
			cout << "연결 시도 중\n";
		} while (host.connect(ip, port) != sf::Socket::Done);

		cout << "-----연결 완료-----\n";
		thread t1(&PlayerSystem::receiveHost, this);
		thread t2(&PlayerSystem::sendHost, this);
		t1.detach();
		t2.detach();
	}
}

void PlayerSystem::draw() {
	window->draw(*mainPlayer);
	for (const auto& player : players)
	{
		window->draw(player.second);
		window->draw(Text(String(player.first), font));
	}
}

void PlayerSystem::listen() {

}

void PlayerSystem::receiveHost() {
	std::string name;
	float x, y;
	float radius;
	Packet data;

	while (true)
	{
		if (host.receive(data) != sf::Socket::Done) {
			std::cout << "호스트의 접속이 종료되었습니다.\n";
			exit(1);
		}

		for (int i = 0; i < players.size(); i++)
		{
			data >> name >> x >> y >> radius;
			players[name].setPosition(x, y);
			players[name].setRadius(radius);
		}
	}
}

void PlayerSystem::sendHost() {
	Clock clock;
	Vector2f pos;
	Packet data;
	while (true)
	{
		if (clock.getElapsedTime().asMilliseconds() >= 20) {
			clock.restart();
			data.clear();
			pos = mainPlayer->getPosition();
			data << mainPlayer->getPlayerName() << pos.x << pos.y << mainPlayer->getRadius();
		}
	}
}

void PlayerSystem::receiveClients()
{
}

void PlayerSystem::sendClients()
{
}
