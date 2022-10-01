#pragma once
#include "particle.h"
#include "mainPlayer.h"
#include <vector>
using std::vector;
class ParticleSystem {
private:
	vector<Particle> particles;
	RenderWindow& window;
	MainPlayer& mainPlayer;
public:
	ParticleSystem(RenderWindow&, MainPlayer&, size_t = 150);
	void draw();
	void update();
};
 