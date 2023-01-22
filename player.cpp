#include "player.h"
#include "system.h"
#include <iostream>

Player::Player(string name) {
	playerImage.setFillColor(System::getRandomColor());

	playerNameText.setCharacterSize(20);
	playerNameText.setString(name);
	playerNameText.setFont(System::defaultFont);
}

Vector2f Player::getConvertedPos() const {
	Vector2f graphicPos = playerImage.getPosition();
	return { graphicPos.x + radius, graphicPos.y + radius };
}

void Player::draw(RenderWindow& window) {
	window.draw(playerImage);
	window.draw(playerNameText);
}

void Player::setPosition(int x, int y) {
	playerNameText.setPosition(x, y);
	playerImage.setPosition(x, y);
}

void Player::setRadius(int radius) {
	playerNameText.setCharacterSize(radius);
	playerImage.setRadius(radius);
}

string Player::getPlayerName() {
	return playerNameText.getString();
}