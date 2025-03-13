#include "Ship.h"
#include "Polyline.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
//#include <iostream>

using namespace std;

double twoPi = glm::pi<double>() * 2;

Ship::Ship()
        : mVelocity(0.0)
        , mAcceleration(0.05)
        , mMaxSpeed(3.0)
        , mPl()
        , mThrusting(false) 
        , mShooting(false) {
    mPl.color = glm::vec4(0.4, 1.0, 0.4, 1.0);

    //mPl.addPoint(-10, 10);
    //mPl.addPoint(0, -20);
    //mPl.addPoint(10, 10);
    //mPl.addPoint(3, 3);
    //mPl.addPoint(-3, 3);


    mPl.addPoint(3, -3);
    mPl.addPoint(0, -10);
    mPl.addPoint(30, 0);
    mPl.addPoint(0, 10);
    mPl.addPoint(3, 3);

    //mPl.addPoint(-10, 10);

    mPl.closed = true;
    mPl.PivotOn(Polyline::PivotCG);
    mPl.width = 2;
    //mPl.scale = 4;
    setRotation(-twoPi / 4);
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

void Ship::update() {
    glm::dvec2 direction(glm::cos(mRotation), glm::sin(mRotation));
    direction = glm::normalize(direction);

    if (mThrusting) {
        mVelocity += mAcceleration * direction;

    }

    if (glm::length(mVelocity) > mMaxSpeed) {
        glm::dvec2 actVelDir = glm::normalize(mVelocity);
        mVelocity = mMaxSpeed * actVelDir;
    }

    //cout << "speed" << glm::length(mVelocity) << ", max speed:" << mMaxSpeed << endl;
    //cout << " direction: " << direction.x << ", " << direction.y << endl;

    switch (mRotating) {
    case -1:
        setRotation(mRotation - twoPi / 64);
        break;
    case 1:
        setRotation(mRotation + twoPi / 64);
        break;
    case 0:
    default:
        break;
    }
    
    glm::dvec2 newPos = mPosition + mVelocity;
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
