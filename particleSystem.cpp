#include "particleSystem.h"
#include "playerSystem.h"

ParticleSystem::ParticleSystem(RenderWindow& window, MainPlayer& player, size_t size) : window(window), mainPlayer(player) {
	particles.resize(size);
}

void ParticleSystem::draw() {
	for (const auto& particle : particles) {
		if (particle.getActiveState())
			window.draw(particle);
	}
}

void ParticleSystem::update() {
	for (auto& particle : particles) {
		if (!particle.getActiveState()) {
			particle.setActive(true);
		}
		if (mainPlayer.collide(particle.getPos())) {
			particle.setActive(false);
			mainPlayer.grow();
		}
	}
}
