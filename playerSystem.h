#pragma once
#include <SFML/Network.hpp>
#include <thread>
#include <memory>
#include <unordered_map>

#include "player.h"
#include "mainPlayer.h"
#include "particleSystem.h"

using std::thread;
using std::unique_ptr;
using std::make_unique;
using sf::IpAddress;
using sf::TcpSocket;
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
	std::unordered_map < std::string, Player> players;
	RenderWindow* window;

	Font font;
	IpAddress ip = IpAddress::getLocalAddress();
	unsigned short port = 53000;

	unique_ptr<MainPlayer> mainPlayer;
	unique_ptr<TcpSocket> host;
	std::vector<TcpSocket> clients;

	friend void ParticleSystem::update(PlayerSystem*);

	void listen();

public:
	PlayerSystem(RenderWindow*, PlayMode);
	~PlayerSystem();
	void draw();
	void update();
	void receiveHost(); //ȣ��Ʈ�� ������ �����͸� �޴� �Լ�
	void sendHost(); //ȣ��Ʈ���� ������ �����͸� ������ �Լ�
	void receiveClients(); //ȣ��Ʈ�� ��� �����͸� �����ϴ� �Լ�
	void sendClients(); //ȣ��Ʈ�� ��� ������ �����͸� ������ �Լ�
};