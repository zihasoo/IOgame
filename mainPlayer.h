#pragma once
#include <SFML/Graphics.hpp>
#include "player.h"

using sf::RenderWindow;

class MainPlayer : public Player
{
	Vector2f viewSize = { 480,270 };
	float speed = 4;
	RenderWindow* window;
	View* view;
public:
	MainPlayer(RenderWindow*);
	~MainPlayer();
	bool collide(const Vector2f& otherPos) const;
	void grow();
	void move();
};

