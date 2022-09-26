#include "player.h"
#include "system.h"

Player::Player(){
	pos = System::getRandomPos();
	graphicPos = Vector2f(pos.x - 50, pos.y - 50);
	setFillColor(System::getRandomColor());
	setPosition(graphicPos);
	setRadius(radius);
}

void Player::convertGraphicPos() {
	graphicPos = { pos.x - radius, pos.y - radius };
}

void Player::setWorldPos(float x, float y) {
	pos.x = x;
	pos.y = y;
	convertGraphicPos();
	setPosition(graphicPos);
}