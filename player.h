#pragma once
#include <SFML/Graphics.hpp>

using sf::CircleShape;
using sf::RenderWindow;
using sf::View;
using sf::Vector2f;

class Player : public CircleShape {
protected:
	float radius = 20;
public:
	Vector2f getConvertedPos() const;
	Player();
	Player(float, float, float);
};