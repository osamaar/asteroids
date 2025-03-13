#include "Entity.h"
#include <glm/glm.hpp>

Entity::Entity()
        : mPosition(0.0) 
        , mRotation(0.0)
        , mCollisionRadius(0.0) {

}

Entity::~Entity() {

}

glm::dvec2 Entity::getPosition() {
    return mPosition;
}

double Entity::getRotation() {
    return mRotation;
}

void Entity::setPosition(double x, double y) {
    mPosition.x = x;
    mPosition.y = y;
}

void Entity::setRotation(double rotation) {
    mRotation = rotation;
}

bool collides(Entity &e0, Entity &e1) {
    double distance = glm::length(e1.mPosition - e0.mPosition);
    return (distance <= e0.mCollisionRadius + e1.mCollisionRadius);
}
