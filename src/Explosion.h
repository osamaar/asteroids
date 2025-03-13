#ifndef EXPLOSION_H_
#define EXPLOSION_H_


#include "ObjectPool.h"
#include "ParticleSystem.h"
#include "Polyline.h"
#include <glm/glm.hpp>

class PolylineRenderer;

class ExplosionEmitter : public ParticleEmitter {
public:
    ExplosionEmitter();
    virtual ~ExplosionEmitter();
    void init(const glm::dvec2 &pos);
    void render(PolylineRenderer &renderer);
protected:
    struct DebrisShape {
        Polyline pl;
        PoolState poolState;
        DebrisShape() : pl() {
            pl.addPoint(0.0, 0.0);
            pl.addPoint(0.0, 0.0);
        };
    };

    ObjectPool<DebrisShape> mDebris;
    virtual void evolveParticles(int dt) override;
    virtual void initParticle(Particle *p);
private:
    int mNumDebris;
};

class Explosion : public ParticleSystem<ExplosionEmitter> {
public:
    glm::dvec2 position;
    PoolState poolState;

    Explosion();
    virtual ~Explosion();
    void init(const glm::dvec2 &pos);
    void render(PolylineRenderer &plr);
};

#endif // EXPLOSION_H_