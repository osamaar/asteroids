#pragma once


#include "Entity.h"
#include "Polyline.h"
#include "ObjectPool.h"

class Asteroid : public Entity {
public:
    // Change poolState.alive when you kill an asteroid.
    PoolState poolState;

    Asteroid();
    Asteroid(double radius);
    virtual ~Asteroid();

    void setRadius(double radius);
    double getRadius();
    void createShape();
    virtual void setPosition(double x, double y);
    virtual void setRotation(double rotation);      // degrees
    virtual void update();
    virtual void render(PolylineRenderer &renderer);
private:
    Polyline mPl;
    double mRadius;
};

