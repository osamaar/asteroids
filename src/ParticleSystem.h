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
    ParticleEmitter() { /* initial state */ }
    virtual ~ParticleEmitter() { }
    virtual void update(int dt) { /* emit & manage */  }
private:
    // Derived classes declare own particle container
};

class ParticleSystem {
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