#pragma once


#include "Entity.h"
#include "Polyline.h"

class Asteroid : public Entity {
public:
    Asteroid();
    Asteroid(double radius);
    ~Asteroid();

    void setRadius(double radius);
    double getRadius();
    void createShape();
    virtual void setPosition(double x, double y);
    virtual void setRotation(double rotation);      // degrees
    virtual void render(PolylineRenderer &renderer);
private:
    Polyline mPl;
    double mRadius;
};

