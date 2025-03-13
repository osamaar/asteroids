#include "ParticleSystem.h"
#include <fmt/format.h>

//ParticleSystem::ParticleSystem()
//        : mEmitters() {

//}

//void ParticleSystem::update(int dt) {
//    updateEmitters(dt);
//    updateSystem(dt);
//}

//void ParticleSystem::addEmitter(ParticleEmitter & emitter) {
//    mEmitters.push_back(&emitter);
//}

//void ParticleSystem::updateEmitters(int dt) {
//    for (auto &&emitter : mEmitters) {
//        emitter->update(dt);
//    }
//}

void Particle::update(int dt) {
    velocity += acceleration*(dt/1000.0);
    position += velocity*(dt/1000.0);
    age += dt;
    //if (age > maxAge) {
    //    poolState.alive = false;
    //}
}

void ParticleEmitter::update(int dt) {
    age += dt;
    evolveParticles(dt);
    emit(dt);
    mParticles.apply([&](Particle &p) {
        p.update(dt);
    });
}

void ParticleEmitter::reset() {
    age = 0;
    mEmitAccum = 0;
    mParticles.releaseAll();
}

void ParticleEmitter::emit(int dt) {
    mElapsed += dt;
    //fmt::print("1000.0/emitRate: {}, elapsed: {:.4f}\n",
    //            1000.0/emitRate, mElapsed/1000.0);
    if (!emitRate || mElapsed < 1000.0/emitRate) return;
    mEmitAccum += emitRate*dt;
    for (int i = 0; i < mEmitAccum/1000; i++) {
        //fmt::print("emitting {}...", i);
        Particle *p = mParticles.aquireObject();
        if (p) initParticle(p);
    }
    mElapsed = 0;
}
