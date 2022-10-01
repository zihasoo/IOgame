#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include "player.h"

using sf::RenderWindow;
using sf::SoundBuffer;
using sf::Sound;
using std::string;

class MainPlayer : public Player
{
	Vector2f viewSize = { 480,270 };
	float speed = 4;
	RenderWindow& window;
	View* view;
	string playerName;
	
	SoundBuffer* buffer;
	Sound* sound;

public:
	MainPlayer(RenderWindow&,string&);
	~MainPlayer();
	bool collide(const Vector2f& otherPos) const;
	void grow();
	void move();
	string getPlayerName();
};

