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
    virtual void update(int dt) = 0;
    virtual void render(PolylineRenderer &renderer) = 0;
protected:
    glm::dvec2 mPosition;
    double mRotation;
    double mCollisionRadius;

    friend bool collides(Entity &e0, Entity &e1);
};

bool collides(Entity &e0, Entity &e1);
