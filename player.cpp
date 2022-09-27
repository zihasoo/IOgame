#include "player.h"
#include "system.h"
#include <iostream>

Player::Player(){
	setFillColor(System::getRandomColor());
	setRadius(radius);
}

Player::Player(float x, float y, float radius) : radius(radius) {
	setFillColor(System::getRandomColor());
	setPosition(x,y);
	setRadius(radius);
}

Vector2f Player::getConvertedPos() const{
	Vector2f graphicPos = getPosition();
	return { graphicPos.x + radius, graphicPos.y + radius };
}