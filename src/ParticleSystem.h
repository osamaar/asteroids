#ifndef PARTICLESYSTEM_H_
#define PARTICLESYSTEM_H_


#include "ObjectPool.h"
#include <glm/glm.hpp>
#include <vector>

struct Particle {
    glm::dvec2 position;
    glm::dvec2 velocity;
    glm::dvec2 acceleration;
    int age;
    int maxAge;
    PoolState poolState;
    void update(int dt);
};

class ParticleEmitter {
public:
    glm::dvec2 position;
    double emitRate;
    double emitMaxPopulation;
    int emitDecay;
    int age, maxAge;

    ParticleEmitter()
            : position(0.0, 0.0)
            , emitRate(0)
            , emitMaxPopulation(8)
            , emitDecay(1000)
            , mParticles()
            , mElapsed(0)
            , mEmitAccum(0)
            , age(0)
            , maxAge(0) {
        /* initial state */
    }

    virtual ~ParticleEmitter() { }
    void update(int dt);
    void reset();
protected:
    ObjectPool<Particle> mParticles;
    virtual void evolveParticles(int dt) { /* apply forces, etc. */  }
    virtual void initParticle(Particle *p) { }
private:
    int mElapsed;
    double mEmitAccum;
    void emit(int dt);
};

template <typename EmitterType>
class ParticleSystem {
public:
    int age, maxAge;

    ParticleSystem() : mEmitters(), age(0), maxAge(0) { }
    virtual ~ParticleSystem() { }
    void update(int dt) {
        updateEmitters(dt);
        updateSystem(dt);
    }
    void addEmitter(EmitterType &emitter) {
        mEmitters.push_back(&emitter);
    }
    void reset() {
        age = 0;
        for (auto &&emitter : mEmitters) {
            emitter->reset();
        }
    }
protected:
    std::vector<EmitterType *> mEmitters;
    virtual void updateSystem(int dt) { }
private:
    void updateEmitters(int dt) {
        age += dt;
        for (auto &&emitter : mEmitters) {
            emitter->update(dt);
        }
    }
};


#endif // PARTICLESYSTEM_H_
