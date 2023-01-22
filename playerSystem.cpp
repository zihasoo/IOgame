#include "playerSystem.h"
#include <iostream>

using sf::String;
using sf::Clock;
using std::cout;
using std::cin;

Packet& operator<<(Packet& pack, Player& player) {
	auto [x, y] = player.playerImage.getPosition();
	return pack << x << y << player.playerImage.getRadius();
}

Packet& operator>>(Packet& pack, Player& player) {
	float x, y, r;
	pack >> x >> y >> r;
	player.setPosition(x, y);
	player.setRadius(r);
	return pack;
}

PlayerSystem::PlayerSystem(RenderWindow& window, MainPlayer& player, PlayMode playmode) : window(window), mainPlayer(player) {
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

		Packet initPack;
		initPack << player.getPlayerName();

		host->send(initPack);

		cout << "-----���� �Ϸ�-----\n";

		thread t1(&PlayerSystem::receiveHost, this);
		thread t2(&PlayerSystem::sendHost, this);
		t1.detach();
		t2.detach();
	}
}

void PlayerSystem::draw() {
	mainPlayer.draw(window);

	for (auto& _player : players)
	{
		_player.second.draw(window);
	}
}

void PlayerSystem::accept() {
	string playerName;
	Packet initPack;
	while (curClientCount < acceptableClientCount)
	{
		cout << "Ŭ���̾�Ʈ ��� ��\n";
		TcpSocket* newClient = new TcpSocket;
		if (listener->accept(*newClient) != sf::Socket::Done) {
			cout << "Ŭ���̾�Ʈ ���� ����\n";
			exit(1);
		}

		newClient->receive(initPack);
		initPack >> playerName;
		cout << "Ŭ���̾�Ʈ " << playerName << "�� �����Ͽ����ϴ�.\n";

		clients.emplace(playerName, newClient);
		curClientCount++;
	}
}

void PlayerSystem::receiveHost() {
	std::string name, mainPlayerName = mainPlayer.getPlayerName();
	Packet recvData;
	int playerType;
	while (true)
	{
		if (host->receive(recvData) != sf::Socket::Done) {
			std::cout << "ȣ��Ʈ�� ������ ����Ǿ����ϴ�.\n";
			exit(1);
		}

		while (recvData >> name >> playerType)
		{
			if (name == mainPlayerName) {
				if (static_cast<PlayerType>(playerType) == PlayerType::eatenPlayer)
					cout << "�������ϴ�!\n"; //������ �� ȭ�� �����̶� ��� ���� �߰��ؾߵ�
				float x, y, r;
				recvData >> x >> y >> r;
				continue;
			}
			switch ((PlayerType)playerType)
			{
			case PlayerType::newPlayer:
				players[name] = Player(name);
				break;
			case PlayerType::exitPlayer:
			case PlayerType::eatenPlayer:
				players.erase(players.find(name));
				break;
			default:
				recvData >> players[name];
				break;
			}
		}
	}
}

void PlayerSystem::sendHost() {
	Clock clock;
	Packet sendData;
	while (true)
	{
		if (clock.getElapsedTime().asMilliseconds() >= 20) {
			clock.restart();
			sendData.clear();
			sendData << mainPlayer;

			if (host->send(sendData) != sf::Socket::Done) {
				std::cout << "ȣ��Ʈ�� ������ ����Ǿ����ϴ�.\n";
				exit(1);
			}
		}
	}
}

void PlayerSystem::transmissionClients()
{
	string mainPlayerName = mainPlayer.getPlayerName();
	Packet receiveData, sendData;
	map<string, unique_ptr<TcpSocket>>::iterator iter;

	while (true)
	{
		for (iter = clients.begin(); iter != clients.end(); ++iter) {
			auto& name = iter->first;
			if (iter->second->receive(receiveData) != sf::Socket::Done) {

				std::cout << "Ŭ���̾�Ʈ " << name << "�� ������ ����Ǿ����ϴ�.\n";

				players.erase(players.find(name));

				iter = clients.erase(iter);
				if (iter == clients.end()) break;

				sendData << name << static_cast<int>(PlayerType::exitPlayer);

				continue;
			}
			receiveData >> players[name];

			sendData << name << static_cast<int>(PlayerType::defaultPlayer) << players[name];
		}

		sendData << mainPlayerName << static_cast<int>(PlayerType::defaultPlayer) << mainPlayer;

		for (iter = clients.begin(); iter != clients.end(); ++iter) {
			if (iter->second->send(sendData) != sf::Socket::Done) 
				continue; //���⼭ Ż���� �÷��̾ �߰ߵǸ� �׳� �ѱ�
						  //->���� ������ ���� �������� ó���ϴ°� ���ϱ� ����
		}
		sendData.clear();
		
	}
}
