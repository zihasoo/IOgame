#include "mainPlayer.h"
#include "system.h"

using sf::Keyboard;

MainPlayer::MainPlayer(RenderWindow& window, string& name) : Player(name), window(window) {
	playerImage.setRadius(20);
	playerImage.setPosition(System::getRandomPos());

	buffer = new SoundBuffer;
	buffer->loadFromFile("sounds/eat.wav");
	sound = new Sound(*buffer);
	view = View(getConvertedPos(), viewSize);
	window.setView(view);
}

MainPlayer::~MainPlayer() {
	delete sound;
	delete buffer;
}

bool MainPlayer::collide(const Vector2f& otherPos) const {
	Vector2f pos = getConvertedPos();
	return ((pos.x - otherPos.x) * (pos.x - otherPos.x) +
		(pos.y - otherPos.y) * (pos.y - otherPos.y) <= radius * radius);
}

void MainPlayer::grow() {
	radius += 15 / radius;
	viewSize.x += 75.0f * 16 / 9 / radius;
	viewSize.y += 75.0f / radius;
	setRadius(radius);
	view.setSize(viewSize);
	if (System::soundCoolDown()) {
		sound->play();
	}
}

void MainPlayer::move() {
	Vector2f pos = getConvertedPos();
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
	setPosition(pos.x - radius, pos.y - radius);
	view.setCenter(pos);
	window.setView(view);
}