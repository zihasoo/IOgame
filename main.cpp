#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <algorithm>

using namespace sf;

static const int WindowX = 1500;
static const int WindowY = 800;

class RandomManager {
	std::random_device rd{};
	std::mt19937 gen{ rd() };
	std::uniform_int_distribution<int> color{ 0,255 };
	std::uniform_int_distribution<int> posX{ 0, WindowX };
	std::uniform_int_distribution<int> posY{ 0, WindowY };
public:
	Color getRandomColor() {
		return Color(color(gen), color(gen), color(gen));
	}
	Vector2f getRandomPos() {
		return Vector2f(posX(gen), posY(gen));
	}
};

class Particle : public CircleShape {
private:
	bool activeSelf;
	static RandomManager randMgr;
	void set() {
		setPointCount(15);
		setFillColor(randMgr.getRandomColor());
		setPosition(randMgr.getRandomPos());
		setRadius(10);
	}
public:
	Particle() {
		activeSelf = true;
		set();
	}

	bool operator<(const Particle& other) {
		return getPos().x < other.getPos().x;
	}

	Vector2f getPos() const {
		Vector2f origin = getPosition();
		int radius = getRadius();
		return Vector2f(origin.x + radius, origin.y +  radius);
	}

	bool getActiveState() const {
		return activeSelf;
	}

	void setActive(bool newState) {
		activeSelf = newState;
		if (activeSelf) {
			set();
		}
	}
};

RandomManager Particle::randMgr = RandomManager();

class Player : public CircleShape {
private:
	Vector2f pos = {450,450};
	Vector2f graphicPos = {400,400};
	float radius = 25;

	void convertGraphicPos() {
		graphicPos = { pos.x - radius, pos.y - radius };
	}

public:
	Player() {
		setFillColor(Color::White);
		setPosition(pos);
		setRadius(radius);
	}


	bool collide(const Vector2f& otherPos) const {
		return ((pos.x - otherPos.x) * (pos.x - otherPos.x) +
			(pos.y - otherPos.y) * (pos.y - otherPos.y) <= getRadius() * getRadius());
	}

	void grow() {
		radius += 10/radius;
		setRadius(radius);
	}

	void move() {
		if (Keyboard::isKeyPressed(Keyboard::Up)
			&& pos.y >= radius) {
			pos.y -= 5;
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)
			&& pos.y <= WindowY - radius) {
			pos.y += 5;
		}
		if (Keyboard::isKeyPressed(Keyboard::Left)
			&& pos.x >= radius) {
			pos.x -= 5;
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)
			&& pos.x <= WindowX - radius) {
			pos.x += 5;
		}
		convertGraphicPos();
		setPosition(graphicPos);
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

	~ParticleSystem() {

	}

	void draw() {
		for (const auto& particle : particles) {
			if (particle.getActiveState())
				window->draw(particle);
		}
	}

	void CollisionDetection(Player& player) {
		for (auto& particle : particles) {
			if (!particle.getActiveState()) {
				particle.setActive(true);
			}
			if (player.collide(particle.getPos())) {
				particle.setActive(false);
				player.grow();
			}
		}
	}
};

int main()
{
	RenderWindow* window = new RenderWindow(VideoMode(WindowX, WindowY), "SFML works!");
	window->setFramerateLimit(144);

	ParticleSystem particleSystem(window, 150);
	Player player;
	
	while (window->isOpen())
	{
		Event event;
		while (window->pollEvent(event))
		{
			if (event.type == Event::Closed)
				window->close();
		}
		window->clear();
		player.move();
		particleSystem.CollisionDetection(player);
		window->draw(player);
		particleSystem.draw();
		window->display();
	}

	return 0;
}