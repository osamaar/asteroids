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
    double rate;
    double maxPopulation;
    int decay;

    ParticleEmitter()
            : rate(0)
            , maxPopulation(8)
            , decay(1000)
            , mParticles()
            , mElapsed(0) {
        /* initial state */
    }

    virtual ~ParticleEmitter() { }
    void update(int dt);
protected:
    ObjectPool<Particle> mParticles;
    virtual void evolveParticles(int dt) { /* apply forces, etc. */  }
    virtual void initParticle(Particle *p) { }
private:
    int mElapsed;
    void emit(int dt);
};

class ParticleSystem {
public:
    ParticleSystem();
    virtual ~ParticleSystem() { }
    void update(int dt);
    void addEmitter(ParticleEmitter &emitter);
protected:
    std::vector<ParticleEmitter *> mEmitters;
    virtual void updateSystem(int dt) { }
private:
    void updateEmitters(int dt);
};


#endif // PARTICLESYSTEM_H_