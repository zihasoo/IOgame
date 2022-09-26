#pragma once
#include <SFML/Graphics.hpp>

class Particle : public sf::CircleShape {
private:
	bool activeSelf;
	void set();
public:
	Particle();
	sf::Vector2f getPos() const;
	bool getActiveState() const;
	void setActive(bool newState);
};