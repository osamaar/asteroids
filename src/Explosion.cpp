#include "Explosion.h"

ExplosionEmitter::ExplosionEmitter()
        : position(0.0, 0.0) {
    mParticles.resize(100);
    rate = 0;
    maxPopulation = 100;
    decay = 2000;
    // Create initial amount of debris.
    for (int i = 0; i < 100; i++) {
        Particle *p = mParticles.aquireObject();
        initParticle(p);
    }
}

ExplosionEmitter::~ExplosionEmitter() {
}

void ExplosionEmitter::evolveParticles(int dt) {
    // Apply a drag force on debris
}

void ExplosionEmitter::initParticle(Particle * p) {
    // Random direction, speed and accel
    // Optional: randomize starting point for more chaotic result.
}

Explosion::Explosion()
        : position(0.0, 0.0) {
    ExplosionEmitter *ee = new ExplosionEmitter();
    mEmitters.push_back(ee);
}

Explosion::~Explosion() {
    if (mEmitters[0]) delete mEmitters[0];
}
