#include "Asteroid.h"
#include "helpers.h"
#include <glm/glm.hpp>

using namespace std;

Asteroid::Asteroid()
        : poolState()
        //, age(0)
        //, maxAge(120.0)
        , hp(1)
        , speed(1*60.0)
        , dirNormal(1.0, 1.0)
        , mPl()
        , mTier(3)
        , mRadius(0) {
    createShape();
}

Asteroid::Asteroid(int tier)
        : poolState()
        //, age(0)
        //, maxAge(120.0)
        , hp(tier)
        , speed(1)
        , dirNormal(1.0, 1.0)
        , mPl()
        , mTier(tier)
        , mRadius(0) {
    createShape();
}

Asteroid::Asteroid(double radius)
        : mPl()
        , mRadius(radius) {
    createShape();
}


Asteroid::~Asteroid() {

}

void Asteroid::setRadius(double radius) {
    mRadius = radius;
}

double Asteroid::getRadius() {
    return mRadius;
}

void Asteroid::createShape() {
    mPl.color = glm::vec4(0.6, 0.6, 0.6, 1.0);

    mRadius = mTier * 10.0;
    mCollisionRadius = mRadius;
    double angleStep = 1.0/16.0 * twoPi;
    double rMin = mRadius * 0.75;
    double rMax = mRadius;
    double dist;
    double px, py;
    for (double i = 0; i < twoPi; i += angleStep) {
        dist = randRangeNaive(rMin, rMax);
        px = glm::cos(i) * dist;
        py = glm::sin(i) * dist;
        mPl.addPoint(px, py);
    }
    //mPl.addPoint(mPl.points[0].x, mPl.points[0].y);

    mPl.closed = true;
    mPl.PivotOn(Polyline::PivotCG);
    mPl.width = 1;
    //mPl.scale = 4;
}

void Asteroid::clearShape() {
    mPl.points.clear();
}

void Asteroid::regen(int tier) {
    mTier = tier;
    clearShape();
    createShape();
    hp = tier;
}

int Asteroid::getTier() {
    return mTier;
}

bool Asteroid::isAlive() {
    return poolState.alive;
}

void Asteroid::takeDamage(int dmg) {
    hp -= dmg;
    if (hp < 1) {
        poolState.alive = false;
    }
}

void Asteroid::render(PolylineRenderer &renderer) {
    mPl.render(renderer);
}

void Asteroid::setPosition(double x, double y) {
    Entity::setPosition(x, y);
    mPl.pos = glm::vec2(x, y);
}

void Asteroid::setRotation(double rotation) {
    Entity::setRotation(rotation);
    mPl.rotation = rotation;
}

void Asteroid::update(int dt) {
    auto newPos = getPosition() + dirNormal*speed*(dt/1000.0);
    setPosition(newPos.x, newPos.y);

    //age += 1.0;
    //if (age > maxAge) {
    //    poolState.alive = false;
    //}
}

