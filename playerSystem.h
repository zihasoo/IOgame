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

	TcpSocket host; //Ŭ���̾�Ʈ�� ������ �� ��� ȣ��Ʈ�� ����� ���� ���

	TcpListener listener; //ȣ��Ʈ�� ������ �� ��� ���ο� Ŭ���̾�Ʈ�� accept�ϱ� ���� ���
	TcpSocket* clients; //ȣ��Ʈ�� ������ �� ��� Ŭ���̾�Ʈ��� ����� ���� ���
	const int acceptableClientCount = 10;

	void accept();

public:
	PlayerSystem(RenderWindow*, MainPlayer*, PlayMode);
	~PlayerSystem();
	void draw();
	void receiveHost(); //ȣ��Ʈ�� ������ �����͸� �޴� �Լ�
	void sendHost(); //ȣ��Ʈ���� ������ �����͸� ������ �Լ�
	void transmissionClients(); //ȣ��Ʈ�� ��� �����͸� �����ϰ� ������ �Լ�
};