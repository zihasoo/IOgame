#include "client.h"

Client::Client(){
	client = new TcpSocket;
}

Client::~Client() {
	delete client;
}

void Client::host() {
	TcpListener listener;
	cout << "���� ��� ��\n";
	if (listener.listen(port) != sf::Socket::Done) {
		cout << "���� �ȵ�\n";
	}
	cout << "��Ʈ ���� �Ϸ�\n";

	cout << "Ŭ���̾�Ʈ ��� ��\n";
	if (listener.accept(*client) != sf::Socket::Done) {
		cout << "Ŭ���̾�Ʈ ���� ����\n";
	}
	cout << "-----���� �Ϸ�-----\n";
}

void Client::connect() {
	do
	{
		cout << "���� �õ� ��\n";
	} while (client->connect(ip, port) != sf::Socket::Done);

	cout << "-----���� �Ϸ�-----\n";
}

void Client::receive() {
	string output;
	Packet data;

	while (true)
	{
		if (client->receive(data) != sf::Socket::Done) {
			cout << "������ ������ ����Ǿ����ϴ�.\n";
			exit(1);
		}

		data >> output;
		cout << "����: " << output << '\n';
	}
}

void Client::send() {
	string input;
	while (true)
	{
		cout << "�۽�: ";
		getline(cin, input);

		Packet data;
		data << input;

		if (client->send(data) != sf::Socket::Done) {
			cout << "������ ������ ����Ǿ����ϴ�.\n";
			exit(1);
		}
	}
}

void Client::run() {
	thread sender(&Client::send,this);
	thread receiver(&Client::receive,this);
	
	sender.detach();
	receiver.join();
}