#include "Asteroid.h"
#include "helpers.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

using namespace std;

const double twoPi = 2.0 * glm::pi<double>();

Asteroid::Asteroid()
        : poolState()
        , mPl()
        , mRadius(40.0) {
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

double Asteroid::getRadius()
{
    return mRadius;
}

void Asteroid::createShape() {
    mPl.color = glm::vec4(1.0, 1.0, 1.0, 1.0);

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
    mPl.width = 2;
    //mPl.scale = 4;
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

void Asteroid::update() {

}

