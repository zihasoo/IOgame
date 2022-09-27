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
		clients = new TcpSocket[acceptableClientCount];
		cout << "연결 대기 중\n";
		if (listener.listen(port) != sf::Socket::Done) {
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
		clients = nullptr;
		do
		{
			cout << "연결 시도 중\n";
		} while (host.connect(ip, port) != sf::Socket::Done);

		cout << "-----연결 완료-----\n";

		Vector2f pos = mainPlayer->getPosition();
		Packet data;
		data << mainPlayer->getPlayerName() << pos.x << pos.y << mainPlayer->getRadius();

		if (host.send(data) != sf::Socket::Done) {
			std::cout << "호스트의 접속이 종료되었습니다.\n";
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
	window->draw(*mainPlayer);
	for (const auto& player : players)
	{
		Text nickname(String(player.first), font);
		nickname.setPosition(player.second.getConvertedPos());
		window->draw(player.second);
		window->draw(nickname);
	}
}

void PlayerSystem::accept() {
	Packet data;
	string name;
	float x, y;
	float radius;
	int clientIdx = 0;
	while (clientIdx < acceptableClientCount)
	{
		cout << "클라이언트 대기 중\n";

		if (listener.accept(clients[clientIdx]) != sf::Socket::Done) {
			cout << "클라이언트 연결 실패\n";
			exit(1);
		}
		cout << "-----연결 완료-----\n";
		

		clients[clientIdx].receive(data); //초기 위치 세팅
		data >> name >> x >> y >> radius;
		players[name] = Player(x, y, radius);

		clientIdx++;
	}
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
				std::cout << "호스트의 접속이 종료되었습니다.\n";
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
				std::cout << "클라이언트의 접속이 종료되었습니다.\n";
				//exit(1);
			}
			sendData.append(receiveData.getData(), receiveData.getDataSize());
		}
		pos = mainPlayer->getPosition();
		sendData << mainPlayer->getPlayerName() << pos.x << pos.y << mainPlayer->getRadius();
		//combine and send data
		for (int i = 0; i < players.size(); i++) {
			if (clients[i].send(sendData) != sf::Socket::Done) {
				std::cout << "클라이언트의 접속이 종료되었습니다.\n";
				//exit(1);
			}
		}
		for (int i = 0; i < players.size() + 1; i++)
		{
			sendData >> name >> x >> y >> radius;
			players[name].setPosition(x, y);
			players[name].setRadius(radius);
		}
	}
}

