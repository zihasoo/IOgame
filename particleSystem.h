#pragma once
#include "particle.h"
#include "mainPlayer.h"
#include <vector>

class ParticleSystem {
private:
	std::vector<Particle> particles;
	RenderWindow* window;

public:
	ParticleSystem(RenderWindow*, size_t = 150);
	void draw();
	void update(PlayerSystem* player);
};
