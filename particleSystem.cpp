#include "particleSystem.h"
#include "playerSystem.h"

ParticleSystem::ParticleSystem(RenderWindow* window, size_t size) : window(window) {
	particles.resize(size);
}

void ParticleSystem::draw() {
	for (const auto& particle : particles) {
		if (particle.getActiveState())
			window->draw(particle);
	}
}

void ParticleSystem::update(PlayerSystem* system) {
	for (auto& particle : particles) {
		if (!particle.getActiveState()) {
			particle.setActive(true);
		}
		if (system->mainPlayer->collide(particle.getPos())) {
			particle.setActive(false);
			system->mainPlayer->grow();
		}
	}
}
