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
    virtual void setPosition(double x, double y) override;
    virtual void setRotation(double rotation) override;      // degrees
    virtual void update(int dt) override;
    virtual void render(PolylineRenderer &renderer) override;
private:
    Polyline mPl;
    int mTier;
    double mRadius;
};

