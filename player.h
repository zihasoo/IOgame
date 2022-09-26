#pragma once
#include <SFML/Graphics.hpp>

using sf::CircleShape;
using sf::RenderWindow;
using sf::View;
using sf::Vector2f;

class Player : public CircleShape {
protected:
	Vector2f pos;
	Vector2f graphicPos;
	float radius = 20;
	void convertGraphicPos();

public:
	Player();
	void setWorldPos(float, float);
};