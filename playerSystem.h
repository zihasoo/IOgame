#pragma once
#include <SFML/Network.hpp>
#include <thread>
#include <memory>
#include <unordered_map>
#include <list>

#include "player.h"
#include "mainPlayer.h"

using std::list;
using std::unordered_map;
using std::thread;
using std::unique_ptr;
using std::make_unique;

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

//"Ȯ���� �� ���� �ܺ� ��ȣ" ���� -> �Լ� ���� �ϰ� ���� ���ϸ� �׷��� ��

class PlayerSystem
{
	unordered_map <string, Player> players{};
	RenderWindow& window;

	unique_ptr<Font> font;
	IpAddress ip = IpAddress::getLocalAddress();
	unsigned short port = 53000;

	MainPlayer& mainPlayer;
	unique_ptr<Text> mainPlayerNickName;

	unique_ptr<TcpSocket> host; //Ŭ���̾�Ʈ�� ������ �� ��� ȣ��Ʈ�� ����� ���� ���

	unique_ptr<TcpListener> listener; //ȣ��Ʈ�� ������ �� ��� ���ο� Ŭ���̾�Ʈ�� accept�ϱ� ���� ���
	list<TcpSocket> clients{}; //ȣ��Ʈ�� ������ �� ��� Ŭ���̾�Ʈ��� ����� ���� ���

	const int acceptableClientCount = 10;
	int curClientCount = 0;

	void accept();

public:
	PlayerSystem(RenderWindow&, MainPlayer&, PlayMode);
	void draw();
	void receiveHost(); //ȣ��Ʈ�� ������ �����͸� �޴� �Լ�
	void sendHost(); //ȣ��Ʈ���� ������ �����͸� ������ �Լ�
	void transmissionClients(); //ȣ��Ʈ�� ��� �����͸� �����ϰ� ������ �Լ�
};