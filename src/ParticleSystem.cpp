#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
        : mEmitters() {

}

void ParticleSystem::update(int dt) {
    updateEmitters(dt);
    updateSystem(dt);
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
    if (age > maxAge) {
        poolState.alive = false;
    }
}

void ParticleEmitter::update(int dt) {
    evolveParticles(dt);
    emit(dt);
    mParticles.apply([&](Particle &p) {
        p.update(dt);
    });
}

void ParticleEmitter::emit(int dt) {
    mElapsed += dt;
    if (rate && mElapsed < 1000.0/rate) return;
    Particle *p = mParticles.aquireObject();
    initParticle(p);
    mElapsed = 0;
}
