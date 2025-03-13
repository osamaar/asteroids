#include "Bullet.h"

Bullet::Bullet()
        : poolState()
        , mPl() {
    // a triangle
    mPl.addPoint(-3, -2);
    mPl.addPoint( 3, -1);
    mPl.addPoint( 3,  1);
    mPl.addPoint(-3,  2);
    mPl.scale = 10;
    mPl.closed = true;
    mPl.color = glm::vec4(0.1, 0.1, 1.0, 1.0);
}

Bullet::~Bullet() {

}

void Bullet::setPosition(double x, double y) {
    Entity::setPosition(x, y);
    mPl.pos = glm::vec2(x, y);
}

void Bullet::setRotation(double rotation) {
    Entity::setRotation(rotation);
    mPl.rotation = rotation;
}

void Bullet::update() {

}

void Bullet::render(PolylineRenderer & renderer) {
    mPl.render(renderer);
}
