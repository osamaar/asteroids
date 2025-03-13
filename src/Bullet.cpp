#include "Bullet.h"

Bullet::Bullet()
        : poolState()
        , age(0)
        , maxAge(1.0)
        , speed(600.0)
        , dirNormal(1.0, 1.0)
        , mPl() {
    mCollisionRadius = 3;
    mPl.addPoint(-3, -2);
    mPl.addPoint( 3, -1);
    mPl.addPoint( 3,  1);
    mPl.addPoint(-3,  2);
    //mPl.scale = 1.5;
    mPl.closed = true;
    mPl.color = glm::vec4(0.2, 0.3, 1.0, 1.0);
    mPl.width = 1;
    //mPl.PivotOn(Polyline::PivotCG);
    mPl.PivotOn(Polyline::PivotOrigin);
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

void Bullet::update(int dt) {
    auto newPos = getPosition() + dirNormal*speed*(dt/1000.0);
    setPosition(newPos.x, newPos.y);

    age += dt;
    if (age > maxAge*1000.0) {
        poolState.alive = false;
    }
}

void Bullet::render(PolylineRenderer & renderer) {
    mPl.render(renderer);
}

