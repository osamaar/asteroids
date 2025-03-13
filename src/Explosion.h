#pragma once


#include "ObjectPool.h"
#include "Entity.h"
#include "Polyline.h"
#include <vector>

class Explosion : public Entity {
public:
    PoolState poolState;
    double age;
    double maxAge;

    Explosion();
    ~Explosion();

    void reset(double size, double duration=2.0);
    virtual void update(int dt) override;
    virtual void render(PolylineRenderer &renderer) override;
private:
    int mSize;
    struct ExpFrag {
        Polyline pl;
        double age;
        double maxAge;
        ExpFrag() : pl(), age(0), maxAge(1.0) { }
    };
    std::vector<ExpFrag> mParticles;

    void resetFrags();
};

