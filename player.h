#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#define DEBUG
using sf::Packet;

using sf::CircleShape;
using sf::RenderWindow;
using sf::View;
using sf::Vector2f;
using sf::Text;
using std::string;

class Player {
protected:
	CircleShape playerImage;
	Text playerNameText;
	float radius = 20;
public:
	Player() = default;
	Player(string);
	Vector2f getConvertedPos() const;
	void draw(RenderWindow&);
	void setPosition(int, int);
	void setRadius(int);
	string getPlayerName();

	friend Packet& operator<<(Packet&, Player&);
};