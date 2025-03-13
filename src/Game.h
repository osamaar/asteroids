#pragma once


#include "Explosion.h"
#include "SoundClip.h"
#include "Ship.h"
#include "Asteroid.h"
#include "Bullet.h"
#include "ObjectPool.h"
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <vector>

class Shader;
class PassthroughFilter;
class BlurFilter;
class AddFilter;
class ScreenShakeFilter;
class ScanlineFilter;
class ColorShiftFilter;

class Game {
public:
    Game();
    ~Game();

    void reset();
    void mainloop();
    void handleInput();
    void update(int dt);
    void updateShip(Ship &ship, int dt);
    void updatePlayerBullets(int dt);
    void updateEnemyBullets(int dt);
    void updateAsteroids(int dt);
    void updateExplosions(int dt);
    void wrapAroundScreen(glm::dvec2 &vTarget);
    void generateAsteroids();
    void randomizeAsteroid(Asteroid *a, int tier, double x, double y);
    void loadShaders();
    void loadFilters();
    void unloadShaders();
    void unloadFilters();

    void useAlphaBlending();
    void useAdditiveBlending();
private:
    SDL_Window *mWin;
    SDL_GLContext mContext;
    bool mDone;
    bool mPause;
    bool mShooting;
    Ship mShip;
    ObjectPool<Asteroid> mAsteroidPool;
    ObjectPool<Bullet> mPlayerBulletPool;
    ObjectPool<Explosion> mExplosionPool;
    glm::ivec2 mResolution;
    SoundClip *mShootSound;
    SoundClip *mExplosionSound0;
    SoundClip *mExplosionSound1;

    Shader *plSh, *passSh, *blurSh, *addSh,*screenShakeSh,
           *scanlineSh, *colorShiftSh;
    PassthroughFilter *passFilter;
    BlurFilter *blurFilter;
    AddFilter *addFilter;
    ScanlineFilter *scanlineFilter;
    ScreenShakeFilter *screenShakeFilter;
    ColorShiftFilter *colorShiftFilter;

    Explosion mExplosion;
};

