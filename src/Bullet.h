#pragma once


#include "Entity.h"
#include "ObjectPool.h"
#include "Polyline.h"

class Bullet : public Entity {
public:
    // Change poolState.alive when bullet life is over.
    PoolState poolState;

    Bullet();
    ~Bullet();

    virtual void setPosition(double x, double y);
    virtual void setRotation(double rotation);      // degrees
    virtual void update();
    virtual void render(PolylineRenderer &renderer);
private:
    Polyline mPl;
};

