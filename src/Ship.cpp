#include "Ship.h"
#include "Polyline.h"
#include "ObjectPool.h"
#include "Bullet.h"
#include "helpers.h"
#include <glm/glm.hpp>
#include <iostream>

using namespace std;

Ship::Ship()
        : mVelocity(0.0)
        , mAcceleration(0.05*60.0)
        , mMaxSpeed(10.0*60.0)
        , mPl()
        , mThrusting(false)
        , mShootRate(8.0)
        , mShootElapsed(0){
    mCollisionRadius = 5;
    mPl.color = glm::vec4(0.4, 1.0, 0.4, 1.0);

    //mPl.addPoint(-10, 10);
    //mPl.addPoint(0, -20);
    //mPl.addPoint(10, 10);
    //mPl.addPoint(3, 3);
    //mPl.addPoint(-3, 3);


    mPl.addPoint(-7, -3);
    mPl.addPoint(-10, -10);
    mPl.addPoint(20, 0);
    mPl.addPoint(-10, 10);
    mPl.addPoint(-7, 3);

    mPl.closed = true;
    mPl.width = 2;
    //mPl.scale = 4;
    setRotation(-twoPi / 4);

    // Origin pivot simplifies locating bullets
    mPl.PivotOn(Polyline::PivotOrigin);
    //mPl.PivotOn(Polyline::PivotCG);
}


Ship::~Ship() {

}

// -1 left, 1 rigt, 0 none
void Ship::rotate(int dRotation) {
    mRotating = dRotation;
}

void Ship::thrust(bool toggle) {
    mThrusting = toggle;
}

bool Ship::shoot(ObjectPool<Bullet>& bulletPool, int dt) {
    if (mShootElapsed > 1000.0/mShootRate) {
        mShootElapsed = 0;
        auto bullet = bulletPool.aquireObject();
        if (bullet) {
            bullet->setPosition(mPosition.x, mPosition.y);
            bullet->setRotation(mRotation);
            bullet->age = 0;
            bullet->maxAge = 1.0;
            auto dirVec = glm::dvec2(glm::cos(mRotation), glm::sin(mRotation));
            bullet->speed = 600 + glm::length(mVelocity);
            bullet->dirNormal = glm::normalize(dirVec);
        }
        return true;
    }
    return false;
}

void Ship::setVelocity(double x, double y) {
    mVelocity.x = x;
    mVelocity.y = y;
}

void Ship::update(int dt) {
    mShootElapsed += dt;
    glm::dvec2 direction(glm::cos(mRotation), glm::sin(mRotation));
    direction = glm::normalize(direction);

    if (mThrusting) {
        mVelocity += mAcceleration * direction;

    }

    if (glm::length(mVelocity) > mMaxSpeed) {
        glm::dvec2 actVelDir = glm::normalize(mVelocity);
        mVelocity = mMaxSpeed*actVelDir;
    }

    //cout << "speed" << glm::length(mVelocity) << ", max speed:" << mMaxSpeed << endl;
    //cout << " direction: " << direction.x << ", " << direction.y << endl;

    double rotDt = (twoPi*60.0/64.0)*(dt/1000.0);
    switch (mRotating) {
    case -1:
        setRotation(mRotation - rotDt);
        break;
    case 1:
        setRotation(mRotation + rotDt);
        break;
    case 0:
    default:
        break;
    }
    
    glm::dvec2 newPos = mPosition + mVelocity*(dt/1000.0);
    setPosition(newPos.x, newPos.y);
}

void Ship::setPosition(double x, double y) {
    Entity::setPosition(x, y);
    mPl.pos = glm::vec2(x, y);
}

void Ship::setRotation(double rotation) {
    Entity::setRotation(rotation);
    mPl.rotation = rotation;
}

void Ship::render(PolylineRenderer &renderer) {
    mPl.render(renderer);
}
