#ifndef EXPLOSION_H_
#define EXPLOSION_H_


#include "ParticleSystem.h"
#include <glm/glm.hpp>

class ExplosionEmitter : public ParticleEmitter {
public:
    glm::dvec2 position;

    ExplosionEmitter();
    virtual ~ExplosionEmitter();
protected:
    virtual void evolveParticles(int dt) override;
    virtual void initParticle(Particle *p);
};

class Explosion : public ParticleSystem {
public:
    glm::dvec2 position;

    Explosion();
    virtual ~Explosion();
};

#endif // EXPLOSION_H_