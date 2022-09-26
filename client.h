#pragma once
#include <SFML/Network.hpp>
#include <thread>
#include <string>
#include <iostream>

using std::cout;
using std::cin;
using std::thread;
using std::string;
using sf::IpAddress;
using sf::TcpSocket;
using sf::TcpListener;
using sf::Packet;

class Client
{
	IpAddress ip = IpAddress::getLocalAddress();
	unsigned short port = 53000;
	TcpSocket* client;

	void receive();
	void send();

public:
	Client();
	~Client();
	void host();
	void connect();
	void run();
};