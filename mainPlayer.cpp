#include "mainPlayer.h"
#include "system.h"

using sf::Keyboard;

MainPlayer::MainPlayer(RenderWindow* window) : window(window) {
	view = new View(pos, viewSize);
	window->setView(*view);
}

MainPlayer::~MainPlayer() {
	delete view;
}

bool MainPlayer::collide(const Vector2f& otherPos) const {
	return ((pos.x - otherPos.x) * (pos.x - otherPos.x) +
		(pos.y - otherPos.y) * (pos.y - otherPos.y) <= radius * radius);
}

void MainPlayer::grow() {
	radius += 15 / radius;
	viewSize.x += 75.0f * 16 / 9 / radius;
	viewSize.y += 75.0f / radius;
	setRadius(radius);
	view->setSize(viewSize);
}

void MainPlayer::move() {
	if (Keyboard::isKeyPressed(Keyboard::Up)
		&& pos.y >= radius / 2) {
		pos.y -= speed;
	}
	if (Keyboard::isKeyPressed(Keyboard::Down)
		&& pos.y <= System::MapY - radius / 2) {
		pos.y += speed;
	}
	if (Keyboard::isKeyPressed(Keyboard::Left)
		&& pos.x >= radius / 2) {
		pos.x -= speed;
	}
	if (Keyboard::isKeyPressed(Keyboard::Right)
		&& pos.x <= System::MapX - radius / 2) {
		pos.x += speed;
	}
	convertGraphicPos();
	setPosition(graphicPos);
	view->setCenter(pos);
	window->setView(*view);
}