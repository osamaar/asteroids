#pragma once


#include "ObjectPool.h"
#include "Asteroid.h"
#include "Bullet.h"
#include <SDL2/SDL.h>
#include <vector>

class Game {
public:
    Game();
    ~Game();

    void mainloop();
    void handleInput();
private:
    SDL_Window *mWin;
    SDL_GLContext mContext;
    bool mDone;
    bool mThrusting;
    bool mShooting;
    int mRotating;
    ObjectPool<Asteroid> mAsteroidPool;
    ObjectPool<Bullet> mBulletPool;
};

