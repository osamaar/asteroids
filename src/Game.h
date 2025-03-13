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

    void reset();
    void mainloop();
    void handleInput();
    void update();
    void updateShip(Ship &ship);
    void updatePlayerBullets();
    void updateEnemyBullets();
    void updateAsteroids();
private:
    SDL_Window *mWin;
    SDL_GLContext mContext;
    bool mDone;
    bool mPause;
    bool mShooting;
    Ship mShip;
    ObjectPool<Asteroid> mAsteroidPool;
    ObjectPool<Bullet> mPlayerBulletPool;

    void wrapAroundScreen(glm::dvec2 &vTarget);
    void generateAsteroids();
};

