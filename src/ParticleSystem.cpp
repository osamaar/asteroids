#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
        : mEmitters() {

}

void ParticleSystem::update(int dt) {
    updateEmitters(dt);
}

void ParticleSystem::addEmitter(ParticleEmitter & emitter) {
    mEmitters.push_back(&emitter);
}

void ParticleSystem::updateEmitters(int dt) {
    for (auto &&emitter : mEmitters) {
        emitter->update(dt);
    }
}

void Particle::update(int dt) {
    velocity += acceleration*(dt/1000.0);
    position += velocity*(dt/1000.0);
    age += dt;
    if (age > maxAge*1000.0) {
        poolState.alive = false;
    }
}
