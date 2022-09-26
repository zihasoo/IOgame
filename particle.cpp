#include "particle.h"
#include "system.h"

void Particle::set() {
	setFillColor(System::getRandomColor());
	setPosition(System::getRandomPos());
}

Particle::Particle() {
	setPointCount(10);
	setRadius(10);
	setActive(true);
}

Vector2f Particle::getPos() const {
	Vector2f origin = getPosition();
	int radius = getRadius();
	return Vector2f(origin.x + radius, origin.y + radius);
}

bool Particle::getActiveState() const {
	return activeSelf;
}

void Particle::setActive(bool newState) {
	activeSelf = newState;
	if (activeSelf) {
		set();
	}
}