#include "Entity.h"
#include <glm/glm.hpp>

Entity::Entity()
        : mPosition(0.0) 
        , mRotation(0.0) {

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

