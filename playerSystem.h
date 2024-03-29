#pragma once
#include <SFML/Network.hpp>
#include <thread>
#include <memory>
#include <map>

#include "player.h"
#include "mainPlayer.h"

using std::map;
using std::thread;
using std::unique_ptr;
using std::make_unique;
using std::move;

using sf::IpAddress;
using sf::TcpSocket;
using sf::TcpListener;
using sf::Packet;
using sf::RenderWindow;
using sf::Font;
using sf::Text;

enum class PlayMode
{
	host,
	client
};

enum class PlayerType {
	newPlayer,
	exitPlayer,
	eatenPlayer,
	defaultPlayer
};

//"확인할 수 없는 외부 기호" 버그 -> 함수 선언만 하고 정의 안하면 그렇게 됨

class PlayerSystem
{
	map <string, Player> players{};
	RenderWindow& window;

	IpAddress ip = IpAddress::getLocalAddress();
	unsigned short port = 53000;

	MainPlayer& mainPlayer;

	unique_ptr<TcpSocket> host; //클라이언트로 게임을 할 경우 호스트와 통신을 위해 사용

	unique_ptr<TcpListener> listener; //호스트로 게임을 할 경우 새로운 클라이언트를 accept하기 위해 사용
	map<string, unique_ptr<TcpSocket>> clients{}; //호스트로 게임을 할 경우 클라이언트들과 통신을 위해 사용

	const int acceptableClientCount = 10;
	int curClientCount = 0;

	void accept();
	void receiveHost(); //호스트가 취합한 데이터를 받는 함수
	void sendHost(); //호스트에게 개인의 데이터를 보내는 함수
	void transmissionClients(); //호스트일 경우 데이터를 취합하고 보내는 함수

public:
	PlayerSystem(RenderWindow&, MainPlayer&, PlayMode);
	void draw();
};