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

		cout << "연결 대기 중\n";
		if (listener->listen(port) != sf::Socket::Done) {
			cout << "연결 안됨\n";
			exit(1); //포트를 변경하는 로직 필요
		}

		cout << "포트 리슨 완료\n";

		thread t1(&PlayerSystem::accept, this);
		thread t2(&PlayerSystem::transmissionClients, this);
		t1.detach();
		t2.detach();
	}

	else {
		host = make_unique<TcpSocket>();
		do
		{
			cout << "연결 시도 중\n";
		} while (host->connect(ip, port) != sf::Socket::Done);

		Packet initPack;
		initPack << player.getPlayerName();

		host->send(initPack);

		cout << "-----연결 완료-----\n";

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
		cout << "클라이언트 대기 중\n";
		TcpSocket* newClient = new TcpSocket;
		if (listener->accept(*newClient) != sf::Socket::Done) {
			cout << "클라이언트 연결 실패\n";
			exit(1);
		}

		newClient->receive(initPack);
		initPack >> playerName;
		cout << "클라이언트 " << playerName << "이 접속하였습니다.\n";

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
			std::cout << "호스트의 접속이 종료되었습니다.\n";
			exit(1);
		}

		while (recvData >> name >> playerType)
		{
			if (name == mainPlayerName) {
				if (static_cast<PlayerType>(playerType) == PlayerType::eatenPlayer)
					cout << "먹혔습니다!\n"; //먹혔을 때 화면 구성이랑 등등 뭔가 추가해야됨
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
				std::cout << "호스트의 접속이 종료되었습니다.\n";
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

				std::cout << "클라이언트 " << name << "의 접속이 종료되었습니다.\n";

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
				continue; //여기서 탈주한 플레이어가 발견되면 그냥 넘김
						  //->위의 데이터 취합 과정에서 처리하는게 편하기 때문
		}
		sendData.clear();
		
	}
}
