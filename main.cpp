#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include "system.h"
#include "particle.h"


class Player : public CircleShape {
private:
	Vector2f pos = {450,450};
	Vector2f graphicPos = {400,400};
	Vector2f viewSize = {600,320};
	float radius = 20;
	float speed = 4;
	RenderWindow* window;
	View* view;

	void convertGraphicPos() {
		graphicPos = { pos.x - radius, pos.y - radius };
	}

public:
	Player(RenderWindow* window) : window(window) {
		view = new View(pos,viewSize);
		window->setView(*view);
		setFillColor(Color::White);
		setPosition(pos);
		setRadius(radius);
	}

	~Player() {
		delete view;
	}

	bool collide(const Vector2f& otherPos) const {
		return ((pos.x - otherPos.x) * (pos.x - otherPos.x) +
			(pos.y - otherPos.y) * (pos.y - otherPos.y) <= radius * radius);
	}

	void draw() {
		window->draw(*this);
	}

	void grow() {
		radius += 15 / radius;
		viewSize.x += 75.0f * 15 / 8 / radius;
		viewSize.y += 75.0f / radius;
		setRadius(radius);
		view->setSize(viewSize);
	}

	void move() {
		if (Keyboard::isKeyPressed(Keyboard::Up)
			&& pos.y >= radius) {
			pos.y -= speed;
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)
			&& pos.y <= System::MapY - radius) {
			pos.y += speed;
		}
		if (Keyboard::isKeyPressed(Keyboard::Left)
			&& pos.x >= radius) {
			pos.x -= speed;
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)
			&& pos.x <= System::MapX - radius) {
			pos.x += speed;
		}
		convertGraphicPos();
		setPosition(graphicPos);
		view->setCenter(pos);
		window->setView(*view);
	}
};

class ParticleSystem {
private:
	std::vector<Particle> particles;
	RenderWindow* window;

public:
	ParticleSystem(RenderWindow* window, size_t size = 150) :window(window) {
		particles.resize(size);
	}

	void draw() {
		for (const auto& particle : particles) {
			if (particle.getActiveState())
				window->draw(particle);
		}
	}

	void update(Player* player) {
		for (auto& particle : particles) {
			if (!particle.getActiveState()) {
				particle.setActive(true);
			}
			else if (player->collide(particle.getPos())) {
				particle.setActive(false);
				player->grow();
			}
		}
	}
};

void play() {
	RenderWindow* window = new RenderWindow(VideoMode(System::WindowX, System::WindowY), "IO Game!");
	window->setFramerateLimit(60);

	Texture* texture = new Texture;
	texture->loadFromFile("images/space.jpg");
	Sprite* sprite = new Sprite(*texture);

	ParticleSystem particleSystem(window, 1500);
	Player* player = new Player(window);

	Event event;

	while (window->isOpen())
	{
		System::printFPS();
		while (window->pollEvent(event))
		{
			if (event.type == Event::Closed)
				window->close();
		}
		window->clear();
		window->draw(*sprite);

		if (Keyboard::isKeyPressed(Keyboard::Space)) {
			for (int i = 0; i < 100; i++) player->grow();
		}

		player->move();
		particleSystem.update(player);
		player->draw();
		particleSystem.draw();

		window->display();
	}

	delete texture;
	delete sprite;
	delete player;
	delete window;
}

int main()
{
	play();
	return 0;
}