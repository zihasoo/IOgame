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

		cout << "���� ��� ��\n";
		if (listener.listen(port) != sf::Socket::Done) {
			cout << "���� �ȵ�\n";
			exit(1); //��Ʈ�� �����ϴ� ���� �ʿ�
		}
		cout << "��Ʈ ���� �Ϸ�\n";
		
		thread t1(&PlayerSystem::accept, this);
		t1.detach();

	}

	else {
		do
		{
			cout << "���� �õ� ��\n";
		} while (host.connect(ip, port) != sf::Socket::Done);

		cout << "-----���� �Ϸ�-----\n";

		Vector2f pos = mainPlayer->getPosition();
		Packet data;
		data << mainPlayer->getPlayerName() << pos.x << pos.y << mainPlayer->getRadius();

		if (host.send(data) != sf::Socket::Done) {
			std::cout << "ȣ��Ʈ�� ������ ����Ǿ����ϴ�.\n";
			exit(1);
		}

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

void PlayerSystem::accept() {
	Packet data;
	string name;
	float x, y;
	float radius;
	cout << "Ŭ���̾�Ʈ ��� ��\n";
	if (listener.accept(clients.back()) != sf::Socket::Done) {
		cout << "Ŭ���̾�Ʈ ���� ����\n";
		exit(1);
	}
	cout << "-----���� �Ϸ�-----\n";

	clients.back().receive(data);
	data >> name >> x >> y >> radius;
	players[name] = Player(x, y, radius);
}

void PlayerSystem::receiveHost() {
	std::string name;
	float x, y;
	float radius;
	Packet data;

	while (true)
	{
		if (host.receive(data) != sf::Socket::Done) {
			std::cout << "ȣ��Ʈ�� ������ ����Ǿ����ϴ�.\n";
			exit(1);
		}

		for (int i = 0; i < players.size()+1; i++)
		{
			data >> name >> x >> y >> radius;
			if (name == mainPlayer->getPlayerName()) continue;
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

			if (host.send(data) != sf::Socket::Done) {
				std::cout << "ȣ��Ʈ�� ������ ����Ǿ����ϴ�.\n";
				exit(1);
			}
		}
	}
}

void PlayerSystem::receiveClients()
{
	Vector2f pos;
	Packet data;
	while (true)
	{

		data.clear();
		pos = mainPlayer->getPosition();

		if (host.receive(data) != sf::Socket::Done) {
			std::cout << "Ŭ���̾�Ʈ�� ������ ����Ǿ����ϴ�.\n";
		}

	}
}

void PlayerSystem::sendClients()
{

}
