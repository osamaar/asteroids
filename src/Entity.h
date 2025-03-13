#pragma once

#include <glm/glm.hpp>

class PolylineRenderer;

class Entity {
public:
    Entity();
    ~Entity();

    glm::dvec2 getPosition();
    double getRotation();      // degrees

    virtual void setPosition(double x, double y);
    virtual void setRotation(double rotation);      // degrees
    virtual void render(PolylineRenderer &renderer) = 0;
protected:
    glm::dvec2 mPosition;
    double mRotation;
};

