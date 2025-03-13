#pragma once


#include "Entity.h"
#include "Polyline.h"
#include "ObjectPool.h"

class PolylineRenderer;
class Bullet;

class Ship : public Entity {
public:
    Ship();
    ~Ship();

    void rotate(int dRotation);
    void thrust(bool toggle);
    bool shoot(ObjectPool<Bullet> &bulletPool, int dt);
    void setVelocity(double x, double y);
    virtual void update(int dt) override;
    virtual void setPosition(double x, double y) override;
    virtual void setRotation(double rotation) override;
    virtual void render(PolylineRenderer &renderer) override;
private:
    glm::dvec2 mVelocity;
    double mAcceleration;
    double mMaxSpeed;
    Polyline mPl;
    bool mThrusting;
    int mRotating;
    double mShootRate;
    double mShootElapsed;
};

