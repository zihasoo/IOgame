#include "client.h"

Client::Client(){
	client = new TcpSocket;
}

Client::~Client() {
	delete client;
}

void Client::host() {
	TcpListener listener;
	cout << "연결 대기 중\n";
	if (listener.listen(port) != sf::Socket::Done) {
		cout << "연결 안됨\n";
	}
	cout << "포트 리슨 완료\n";

	cout << "클라이언트 대기 중\n";
	if (listener.accept(*client) != sf::Socket::Done) {
		cout << "클라이언트 연결 실패\n";
	}
	cout << "-----연결 완료-----\n";
}

void Client::connect() {
	do
	{
		cout << "연결 시도 중\n";
	} while (client->connect(ip, port) != sf::Socket::Done);

	cout << "-----연결 완료-----\n";
}

void Client::receive() {
	string output;
	Packet data;

	while (true)
	{
		if (client->receive(data) != sf::Socket::Done) {
			cout << "상대방의 접속이 종료되었습니다.\n";
			exit(1);
		}

		data >> output;
		cout << "수신: " << output << '\n';
	}
}

void Client::send() {
	string input;
	while (true)
	{
		cout << "송신: ";
		getline(cin, input);

		Packet data;
		data << input;

		if (client->send(data) != sf::Socket::Done) {
			cout << "상대방의 접속이 종료되었습니다.\n";
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