#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Particle : public CircleShape {
private:
	bool activeSelf;
	void set();
public:
	Particle();

	Vector2f getPos() const;

	bool getActiveState() const;

	void setActive(bool newState);
};