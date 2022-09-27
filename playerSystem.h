#pragma once
#include <SFML/Network.hpp>
#include <thread>
#include <memory>
#include <unordered_map>

#include "player.h"
#include "mainPlayer.h"

using std::thread;

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

class PlayerSystem
{
	std::unordered_map <std::string, Player> players{};
	RenderWindow* window;

	Font font;
	IpAddress ip = IpAddress::getLocalAddress(); //IpAddress("192.168.0.78");
	unsigned short port = 53000;

	MainPlayer* mainPlayer;
	Text mainPlayerNickName;

	TcpSocket host; //클라이언트로 게임을 할 경우 호스트와 통신을 위해 사용

	TcpListener listener; //호스트로 게임을 할 경우 새로운 클라이언트를 accept하기 위해 사용
	TcpSocket* clients; //호스트로 게임을 할 경우 클라이언트들과 통신을 위해 사용
	const int acceptableClientCount = 10;

	void accept();

public:
	PlayerSystem(RenderWindow*, MainPlayer*, PlayMode);
	~PlayerSystem();
	void draw();
	void receiveHost(); //호스트가 취합한 데이터를 받는 함수
	void sendHost(); //호스트에게 개인의 데이터를 보내는 함수
	void transmissionClients(); //호스트일 경우 데이터를 취합하고 보내는 함수
};