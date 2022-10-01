#include "playerSystem.h"
#include <iostream>

using sf::String;
using sf::Clock;
using std::cout;
using std::cin;

PlayerSystem::PlayerSystem(RenderWindow& window, MainPlayer& player, PlayMode playmode) : window(window), mainPlayer(player) {
	font = make_unique<Font>();
	mainPlayerNickName = make_unique<Text>();
	font->loadFromFile("fonts/Pretendard-Regular.ttf");
	mainPlayerNickName->setString(mainPlayer.getPlayerName());
	mainPlayerNickName->setFont(*font);

	if (playmode == PlayMode::host) {
		listener = make_unique<TcpListener>();

		cout << "���� ��� ��\n";
		if (listener->listen(port) != sf::Socket::Done) {
			cout << "���� �ȵ�\n";
			exit(1); //��Ʈ�� �����ϴ� ���� �ʿ�
		}
		cout << "��Ʈ ���� �Ϸ�\n";
		
		thread t1(&PlayerSystem::accept, this);
		thread t2(&PlayerSystem::transmissionClients, this);
		t1.detach();
		t2.detach();
	}

	else {
		host = make_unique<TcpSocket>();
		do
		{
			cout << "���� �õ� ��\n";
		} while (host->connect(ip, port) != sf::Socket::Done);

		cout << "-----���� �Ϸ�-----\n";
		
		thread t1(&PlayerSystem::receiveHost, this);
		thread t2(&PlayerSystem::sendHost, this);
		t1.detach();
		t2.detach();
	}
}

void PlayerSystem::draw() {
	mainPlayerNickName->setCharacterSize(mainPlayer.getRadius());
	mainPlayerNickName->setPosition(mainPlayer.getPosition());

	window.draw(mainPlayer);
	window.draw(*mainPlayerNickName);

	for (const auto& _player : players)
	{
		Text nickname(_player.first, *font, _player.second.getRadius());
		nickname.setPosition(_player.second.getPosition());
		window.draw(_player.second);
		window.draw(nickname);
	}
}

void PlayerSystem::accept() {
	Packet data;
	string name;
	while (curClientCount< acceptableClientCount)
	{
		cout << "Ŭ���̾�Ʈ ��� ��\n";
		TcpSocket *newClient = new TcpSocket;
		if (listener->accept(*newClient) != sf::Socket::Done) {
			cout << "Ŭ���̾�Ʈ ���� ����\n";
			exit(1);
		}
		cout << "-----���� �Ϸ�-----\n";
		clients.emplace_back(newClient);
		curClientCount++;
	}
}

void PlayerSystem::receiveHost() {
	std::string name, mainPlayerName = mainPlayer.getPlayerName();
	float x, y;
	float radius;
	Packet data;
	
	while (true)
	{
		if (host->receive(data) != sf::Socket::Done) {
			std::cout << "ȣ��Ʈ�� ������ ����Ǿ����ϴ�.\n";
			exit(1);
		}

		while (data >> name >> x >> y >> radius)
		{
			if (name == mainPlayerName) continue;
			players[name].setPosition(x, y);
			players[name].setRadius(radius);
		}
	}
}

void PlayerSystem::sendHost() {
	Clock clock;
	Vector2f pos;
	Packet data;
	string mainPlayerName = mainPlayer.getPlayerName();
	while (true)
	{
		if (clock.getElapsedTime().asMilliseconds() >= 20) {
			clock.restart();
			data.clear();
			pos = mainPlayer.getPosition();
			data << mainPlayerName << pos.x << pos.y << mainPlayer.getRadius();

			if (host->send(data) != sf::Socket::Done) {
				std::cout << "ȣ��Ʈ�� ������ ����Ǿ����ϴ�.\n";
				exit(1);
			}
		}
	}
}

void PlayerSystem::transmissionClients()
{
	Vector2f pos;
	string name;
	float x, y, radius;
	Packet receiveData, sendData;
	while (true)
	{
		sendData.clear();
		//data receive and combine
		for (auto& client : clients) {
			if (client->receive(receiveData) != sf::Socket::Done) {
				std::cout << "Ŭ���̾�Ʈ�� ������ ����Ǿ����ϴ�.\n";
				exit(1);
			}
			receiveData >> name >> x >> y >> radius;
			players[name].setPosition(x, y);
			players[name].setRadius(radius);

			sendData << name << x << y << radius;
		}

		pos = mainPlayer.getPosition();
		sendData << mainPlayer.getPlayerName() << pos.x << pos.y << mainPlayer.getRadius();

		//send data
		for (auto& client : clients) {
			if (client->send(sendData) != sf::Socket::Done) {
				std::cout << "Ŭ���̾�Ʈ�� ������ ����Ǿ����ϴ�.\n";
				exit(1);
			}
		}
	}
}
