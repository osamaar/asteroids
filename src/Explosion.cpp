#include "Explosion.h"
#include "PolylineRenderer.h"
#include "helpers.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <fmt/format.h>

ExplosionEmitter::ExplosionEmitter()
        : mNumDebris(200) {
    //init(position);
    //emitRate = 0;
}

ExplosionEmitter::~ExplosionEmitter() {
}

void ExplosionEmitter::init(const glm::dvec2 &pos) {
    ParticleEmitter::position = pos;
    ParticleEmitter::emitRate = 0;
    ParticleEmitter::emitMaxPopulation = mNumDebris;
    ParticleEmitter::emitDecay = 2000;

    mParticles.resize(mNumDebris);
    mDebris.resize(mNumDebris);

    // Create initial amount of debris.
    mParticles.releaseAll();
    for (int i = 0; i < mNumDebris; i++) {
        Particle *p = mParticles.aquireObject();
        if (p) initParticle(p);
    }
}

void ExplosionEmitter::render(PolylineRenderer &renderer) {
    mDebris.apply([&](DebrisShape &d) {
        renderer.render(d.pl);
    });
}

void ExplosionEmitter::evolveParticles(int dt) {
    // Set up a DebrisShape for each particle. Not ideal.
    mDebris.releaseAll();
    mParticles.apply([&](Particle &p) {
        // Particle *tail* is located at particle position.
        // Particle *head* trails it's head by an amount proportional
        // to its velocity.
        if (p.age > p.maxAge) {
            mParticles.releaseObject(p);
            return;
        }
        DebrisShape *d = mDebris.aquireObject();
        double len = glm::length(p.velocity);
        double scale = 0.3;
        double phase = (double)p.age/p.maxAge;
        //fmt::print("particle:: age: {:d}, maxAge: {:d}, phase: {:.4f}\n", 
        //           p.age, p.maxAge, phase);
        glm::dvec2 u = glm::normalize(p.velocity);
        d->pl.points[0] = p.position;
        d->pl.points[1] = p.position + u*(len*scale);
        d->pl.color = glm::vec4(0.5 - 0.2*(1-phase),
                                0.3*(1-phase),
                                0.6*phase,
                                0.25*(1-phase));
        d->pl.width = 3;
    });
    // Apply a drag force on debris
}

void ExplosionEmitter::initParticle(Particle * p) {
    // Random direction, speed and accel
    // Optional: randomize starting point for more chaotic result.
    // TODO: Figure out screen units.
    double velocityFactor = randRangeNaive(35.0, 60.0);
    double deccelFactor = randRangeNaive(-0.25, -0.15);
    double angle = randRangeNaive(0.0, 2.0*glm::pi<double>());
    p->position = position;
    p->velocity.x = glm::cos(angle) * velocityFactor;
    p->velocity.y = glm::sin(angle) * velocityFactor;
    p->acceleration = p->velocity * deccelFactor;
    p->age = 0;
    p->maxAge = ParticleEmitter::maxAge;
}

Explosion::Explosion()
        : position(0.0, 0.0) {
    ParticleSystem::age = 0;
    ParticleSystem::maxAge = 5000;
    ExplosionEmitter *ee = new ExplosionEmitter();
    ee->maxAge = ParticleSystem::maxAge;
    mEmitters.push_back(ee);
}

Explosion::~Explosion() {
    if (mEmitters[0]) delete mEmitters[0];
}

void Explosion::init(const glm::dvec2 &pos) {
    ParticleSystem::reset();
    for (auto e : mEmitters) {
        e->init(pos);
    }
}

void Explosion::render(PolylineRenderer & plr) {
    for (auto e : mEmitters) {
        e->render(plr);
    }
}
