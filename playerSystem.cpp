#include "playerSystem.h"
#include <iostream>

using sf::String;
using sf::Clock;
using std::cout;
using std::cin;

PlayerSystem::PlayerSystem(RenderWindow* window, MainPlayer* player, PlayMode playmode) : window(window), mainPlayer(player) {
	font.loadFromFile("fonts/Pretendard-Regular.ttf");
	mainPlayerNickName.setString(mainPlayer->getPlayerName());
	mainPlayerNickName.setFont(font);

	if (playmode == PlayMode::host) {

		clients = new TcpSocket[acceptableClientCount];

		cout << "���� ��� ��\n";
		if (listener.listen(port) != sf::Socket::Done) {
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

		clients = nullptr;

		do
		{
			cout << "���� �õ� ��\n";
		} while (host.connect(ip, port) != sf::Socket::Done);

		cout << "-----���� �Ϸ�-----\n";

		Packet data;
		//�ٸ� �÷��̾� ���� (players�� �ùٸ� �г������� �ֱ⸸ �ϸ� ������ �ٸ� �����尡 �˾Ƽ� ����)
		if (host.receive(data) != sf::Socket::Done) {
			std::cout << "ȣ��Ʈ�� ������ ����Ǿ����ϴ�.\n";
			exit(1);
		}
		string name;
		while (data >> name)
		{
			players[name] = Player();
		}

		//�г��� �۽� 
		data.clear();
		data << mainPlayer->getPlayerName();
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

PlayerSystem::~PlayerSystem() {
	if(clients != nullptr)
		delete[] clients;
}

void PlayerSystem::draw() {
	mainPlayerNickName.setCharacterSize(mainPlayer->getRadius());
	mainPlayerNickName.setPosition(mainPlayer->getPosition());

	window->draw(*mainPlayer);
	window->draw(mainPlayerNickName);

	for (const auto& _player : players)
	{
		Text nickname(_player.first,font, _player.second.getRadius());
		nickname.setPosition(_player.second.getPosition());
		window->draw(_player.second);
		window->draw(nickname);
	}
}

void PlayerSystem::accept() {
	Packet data;
	string name;
	int clientIdx = 0;
	while (clientIdx < acceptableClientCount)
	{
		cout << "Ŭ���̾�Ʈ ��� ��\n";

		if (listener.accept(clients[clientIdx]) != sf::Socket::Done) {
			cout << "Ŭ���̾�Ʈ ���� ����\n";
			exit(1);
		}
		cout << "-----���� �Ϸ�-----\n";

		//������ �÷��̾�� �ٸ� �÷��̾� ���� �۽�
		for (const auto& _player : players) {
			data << _player.first;
		}
		clients[clientIdx].send(data);
		
		data.clear();
		clients[clientIdx].receive(data); //�÷��̾� ���� (������ �����ڸ��� ��ġ �ְ�����ϱ� �̸��� ����)
		data >> name;
		players[name] = Player();

		clientIdx++;
	}
}

void PlayerSystem::receiveHost() {
	std::string name, mainPlayerName = mainPlayer->getPlayerName();
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
	string mainPlayerName = mainPlayer->getPlayerName();
	while (true)
	{
		if (clock.getElapsedTime().asMilliseconds() >= 20) {
			clock.restart();
			data.clear();
			pos = mainPlayer->getPosition();
			data << mainPlayerName << pos.x << pos.y << mainPlayer->getRadius();

			if (host.send(data) != sf::Socket::Done) {
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
		//data receive
		for (int i = 0; i < players.size(); i++) {
			if (clients[i].receive(receiveData) != sf::Socket::Done) {
				std::cout << "Ŭ���̾�Ʈ�� ������ ����Ǿ����ϴ�.\n";
				exit(1);
			}
			receiveData >> name >> x >> y >> radius;
			players[name].setPosition(x, y);
			players[name].setRadius(radius);

			sendData << name << x << y << radius;
		}

		pos = mainPlayer->getPosition();
		sendData << mainPlayer->getPlayerName() << pos.x << pos.y << mainPlayer->getRadius();

		//combine and send data
		for (int i = 0; i < players.size(); i++) {
			if (clients[i].send(sendData) != sf::Socket::Done) {
				std::cout << "Ŭ���̾�Ʈ�� ������ ����Ǿ����ϴ�.\n";
				exit(1);
			}
		}
	}
}

