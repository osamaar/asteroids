#pragma once


#include "Ship.h"
#include "Asteroid.h"
#include "Bullet.h"
#include "ObjectPool.h"
#include <SDL2/SDL.h>
#include <vector>

class Game {
public:
    Game();
    ~Game();

    void mainloop();
    void handleInput();
    void updateShip(Ship &ship);
    void updatePlayerBullets();
    void updateEnemyBullets();
    void updateAsteroids();
private:
    SDL_Window *mWin;
    SDL_GLContext mContext;
    bool mDone;
    bool mThrusting;
    bool mShooting;
    int mRotating;
    ObjectPool<Asteroid> mAsteroidPool;
    ObjectPool<Bullet> mPlayerBulletPool;

    void wrapAroundScreen(glm::dvec2 &vTarget);
    void generateAsteroids();
};

