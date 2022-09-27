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

enum class PlayMode
{
	host,
	client
};

class PlayerSystem
{
	std::unordered_map <std::string, Player> players;
	RenderWindow* window;

	Font font;
	IpAddress ip = IpAddress::getLocalAddress();
	unsigned short port = 53000;

	MainPlayer* mainPlayer;
	TcpSocket host;
	TcpListener listener;
	std::vector<TcpSocket> clients{1};

	void accept();

public:
	PlayerSystem(RenderWindow*, MainPlayer*, PlayMode);
	void draw();
	void receiveHost(); //ȣ��Ʈ�� ������ �����͸� �޴� �Լ�
	void sendHost(); //ȣ��Ʈ���� ������ �����͸� ������ �Լ�
	void receiveClients(); //ȣ��Ʈ�� ��� �����͸� �����ϴ� �Լ�
	void sendClients(); //ȣ��Ʈ�� ��� ������ �����͸� ������ �Լ�
};