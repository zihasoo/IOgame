#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "player.h"

using sf::RenderWindow;
using sf::SoundBuffer;
using sf::Sound;

class MainPlayer : public Player
{
#ifdef DEBUG
	Vector2f viewSize = { 3840 , 2160 };
#else
	Vector2f viewSize = { 480,270 };
#endif // DEBUG

	float speed = 4;
	RenderWindow& window;
	View view;

	SoundBuffer* buffer;
	Sound* sound;

public:
	MainPlayer(RenderWindow&, string&);
	~MainPlayer();
	bool collide(const Vector2f& otherPos) const;
	void grow();
	void move();
};

