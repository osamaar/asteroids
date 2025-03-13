#pragma once


#include "Entity.h"
#include "Polyline.h"

class PolylineRenderer;

class Ship : public Entity {
public:
    Ship();
    ~Ship();

    void rotate(int dRotation);
    void thrust(bool toggle);
    void update();
    virtual void setPosition(double x, double y);
    virtual void setRotation(double rotation);
    virtual void render(PolylineRenderer &renderer);
private:
    glm::dvec2 mVelocity;
    double mAcceleration;
    double mMaxSpeed;
    Polyline mPl;
    bool mThrusting;
    bool mShooting;
    int mRotating;
};

