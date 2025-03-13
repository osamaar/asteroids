#pragma once


#include "Entity.h"
#include "Polyline.h"
#include "ObjectPool.h"

class Asteroid : public Entity {
public:
    // Change poolState.alive when you kill an asteroid.
    PoolState poolState;
    //double age;
    //double maxAge;
    double speed;
    glm::dvec2 dirNormal;

    Asteroid();
    Asteroid(int tier);
    Asteroid(double radius);
    virtual ~Asteroid();

    void setRadius(double radius);
    double getRadius();
    void createShape();
    void clearShape();
    void regenShape(int tier);
    int getTier();
    virtual void setPosition(double x, double y);
    virtual void setRotation(double rotation);      // degrees
    virtual void update();
    virtual void render(PolylineRenderer &renderer);
private:
    Polyline mPl;
    int mTier;
    double mRadius;
};

