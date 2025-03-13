#pragma once


#include "Entity.h"
#include "ObjectPool.h"
#include "Polyline.h"
#include <glm/glm.hpp>

class Bullet : public Entity {
public:
    // Change poolState.alive when bullet life is over.
    PoolState poolState;
    double age;
    double maxAge;
    double speed;
    glm::dvec2 dirNormal;

    Bullet();
    ~Bullet();

    virtual void setPosition(double x, double y) override;
    virtual void setRotation(double rotation) override;      // degrees
    virtual void update() override;
    virtual void render(PolylineRenderer &renderer) override;
private:
    Polyline mPl;
};

