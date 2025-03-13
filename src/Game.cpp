#include "ColorShiftFilter.h"
#include "ScanlineFilter.h"
#include "ScreenShakeFilter.h"
#include "SoundClip.h"
#include "AddFilter.h"
#include "BlurFilter.h"
#include "PassthroughFilter.h"
#include "Game.h"
#include "Ship.h"
#include "Asteroid.h"
#include "Bullet.h"
#include "Polyline.h"
#include "PolylineRenderer.h"
#include "Shader.h"
#include "helpers.h"
#include <glm/glm.hpp>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/GL.h>

const int WIN_W = 800;
const int WIN_H = 600;

Game::Game()
        : mDone(false)
        , mPause(false)
        , mShooting(false)
        , mShip()
        , mAsteroidPool(100)
        , mPlayerBulletPool(100)
        , mExplosionPool(100) {

    int success = false;
    success = SDL_Init(SDL_INIT_EVERYTHING);
    if (success < 0) return;

    success = Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
    if (success < 0) return;

    success = Mix_Init(MIX_INIT_MP3);
    if (success == 0) return;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

    SDL_DisplayMode dispMode;
    SDL_GetDesktopDisplayMode(0, &dispMode);
    mResolution.x = dispMode.w;
    mResolution.y = dispMode.h;

    mWin = SDL_CreateWindow("Asteroids", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, mResolution.x, mResolution.y, SDL_WINDOW_OPENGL);

    mContext = SDL_GL_CreateContext(mWin);
    SDL_GL_SetSwapInterval(1);		// vsync 0:off - 1:on

    glewInit();

    glEnable(GL_DEBUG_OUTPUT);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glEnable(GL_MULTISAMPLE);

    glViewport(0, 0, mResolution.x, mResolution.y);
    SDL_ShowCursor(SDL_DISABLE);

    Mix_AllocateChannels(32);
    Mix_Volume(-1, 128);
    Mix_VolumeMusic(40);

    mShootSound = new SoundClip("res/shoot.wav");
    mShootSound->setVolume(35);
    mExplosionSound0 = new SoundClip("res/explosion-a.wav");
    mExplosionSound0->setVolume(50);
    mExplosionSound1 = new SoundClip("res/explosion-b.wav");
    mExplosionSound1->setVolume(60);

    loadShaders();
    loadFilters();
}

Game::~Game() {
    unloadFilters();
    unloadShaders();

    SDL_ShowCursor(SDL_ENABLE);
    Mix_HaltMusic();
    Mix_HaltChannel(-1);
    if (mShootSound) delete mShootSound;

    Mix_Quit();
    SDL_GL_DeleteContext(mContext);
    SDL_DestroyWindow(mWin);
    SDL_Quit();
}

void Game::reset() {
    mShip.setPosition(mResolution.x/2, mResolution.y/2);
    mShip.setRotation(glm::radians<double>(90));
    mShip.setVelocity(0, 0);
    mAsteroidPool.releaseAll();
    mPlayerBulletPool.releaseAll();
    generateAsteroids();
    mPause = false;
}

void Game::mainloop() {
    PolylineRenderer plr(*plSh, mResolution);

    Mix_Music *music = Mix_LoadMUS("res/drone.mp3");
    if (music) Mix_PlayMusic(music, -1);


    reset();

    int dt = 1;
    int lastFrame = 0;
    while (!mDone) {
        dt = SDL_GetTicks() - lastFrame;
        lastFrame = SDL_GetTicks();
        // Init.
        //mThrusting = false;
        //mShooting = false;
        //mRotating = 0;

        // Input.
        handleInput();

        // Update.
        update(dt);

        // Draw.
        glEnable(GL_MULTISAMPLE);
        passFilter->bind();
        useAlphaBlending();
        glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        plr.begin();
        mShip.render(plr);
        //asteroid->render(plr);

        mAsteroidPool.apply([&](Asteroid& a) {
            if (a.poolState.alive) {
                a.render(plr);
            }
        });

        mPlayerBulletPool.apply([&](Bullet& b) {
            b.render(plr);
        });

        useAdditiveBlending();
        mExplosion.render(plr);
        mExplosionPool.apply([&](Explosion &e) {
            e.render(plr);
        });

        plr.end();

        useAlphaBlending();
        
        passFilter->process();
        passFilter->unbind();

        blurFilter->bind();
        passFilter->renderContent();
        blurFilter->process();
        blurFilter->unbind();

        addFilter->bind();
        passFilter->renderContent();
        // must get blurred texture every frame due to its alternating buffers
        addFilter->bindAddFramebuffer();
        blurFilter->renderContent();
        addFilter->process();
        addFilter->unbind();

        screenShakeFilter->bind();
        addFilter->renderContent();
        screenShakeFilter->process();
        screenShakeFilter->unbind();

        colorShiftFilter->bind();
        screenShakeFilter->renderContent();
        colorShiftFilter->process();
        colorShiftFilter->unbind();

        scanlineFilter->bind();
        colorShiftFilter->renderContent();
        //screenShakeFilter->renderContent();
        scanlineFilter->process();
        scanlineFilter->unbind();

        // render to backbuffer
        glEnable(GL_MULTISAMPLE);
        //glClearColor(0.2, 0.2, 0.2, 0.0);
        //glClear(GL_COLOR_BUFFER_BIT);
        scanlineFilter->renderContent();

        // render to backbuffer again
        //useAdditiveBlending();
        //blurFilter.renderContent();

        SDL_GL_SwapWindow(mWin);
    }
}

void Game::handleInput() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
        	mDone = true;
        } else if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                mDone = true;
            } else if (e.key.keysym.sym == SDLK_PAUSE) {
                mPause = !mPause;
            } else if (e.key.keysym.sym == SDLK_RETURN) {
                reset();
            } else if (e.key.keysym.sym == SDLK_UP) {
                mShip.thrust(true);
            } else if (e.key.keysym.sym == SDLK_LEFT) {
                mShip.rotate(-1);
            } else if (e.key.keysym.sym == SDLK_RIGHT) {
                mShip.rotate(1);
            } else if (e.key.keysym.sym == SDLK_SPACE) {
                mShooting = true;
            }
        } else if (e.type == SDL_KEYUP) {
            if (e.key.keysym.sym == SDLK_UP) {
                mShip.thrust(false);
            } else if (e.key.keysym.sym == SDLK_LEFT) {
                mShip.rotate(0);
            } else if (e.key.keysym.sym == SDLK_RIGHT) {
                mShip.rotate(0);
            } else if (e.key.keysym.sym == SDLK_SPACE) {
                mShooting = false;
            } else if (e.key.keysym.sym == SDLK_HOME) {
                screenShakeFilter->init(0.05);
            } else if (e.key.keysym.sym == SDLK_DELETE) {
                mExplosion.init(glm::dvec2(200, 100));
            }
        }
    }
}

void Game::update(int dt) {
    if (!mPause) {
        updateShip(mShip, dt);
        updatePlayerBullets(dt);
        updateEnemyBullets(dt);
        updateAsteroids(dt);
        updateExplosions(dt);
        screenShakeFilter->update(dt);
        colorShiftFilter->update(dt);
    }

    if (mAsteroidPool.sizeActive() == 0) {
        mPause = true;
    }
}

void Game::updateShip(Ship &ship, int dt) {
    bool shotFired = false;
    if (mShooting) {
        shotFired = ship.shoot(mPlayerBulletPool, dt);
        if (shotFired) {
            screenShakeFilter->init(0.007);
            mShootSound->play();
        }
    }

    ship.update(dt);

    auto shipPos = ship.getPosition();
    wrapAroundScreen(shipPos);
    ship.setPosition(shipPos.x, shipPos.y);

    mAsteroidPool.apply([&](Asteroid& a) {
        if (collides(a, ship)) {
            mPause = true;
            mExplosionSound1->play();
        }
    });

}

void Game::updatePlayerBullets(int dt) {
    mPlayerBulletPool.apply([&](Bullet& b) {
        b.update(dt);
        if (b.age > b.maxAge) {
            mPlayerBulletPool.releaseObject(b);
        }
        auto bPos = b.getPosition();
        wrapAroundScreen(bPos);
        b.setPosition(bPos.x, bPos.y);

        mAsteroidPool.resizeAtLeast(mAsteroidPool.sizeActive()*3);
        mAsteroidPool.apply([&](Asteroid& a) {
            if (!collides(a, b)) return;

            int tier = a.getTier() - 1;
            auto pos = a.getPosition();
            a.takeDamage(1);

            mPlayerBulletPool.releaseObject(b);

            if (a.isAlive()) return;

            // Create a new explosion.
            Explosion *e = mExplosionPool.aquireObject();
            if (e) e->init(a.getPosition());

            mAsteroidPool.releaseObject(a);
            mExplosionSound0->play();
            screenShakeFilter->init(0.05);
            colorShiftFilter->init(0.01);

            if (tier > 0) {
                for (int i = 0; i < 3; i++) {
                    auto a = mAsteroidPool.aquireObject();
                    if (a) randomizeAsteroid(a, tier, pos.x, pos.y);
                }
            }
        });
    });
}

void Game::updateEnemyBullets(int dt) {

}

void Game::updateAsteroids(int dt) {
    mAsteroidPool.apply([&](Asteroid& a) {
        a.update(dt);
        auto aPos = a.getPosition();
        wrapAroundScreen(aPos);
        a.setPosition(aPos.x, aPos.y);
    });

}

void Game::updateExplosions(int dt) {
    mExplosion.update(dt);
    mExplosionPool.apply([&](Explosion &e) {
        if (e.age > e.maxAge) {
            mExplosionPool.releaseObject(e);
        }
        e.update(dt);
    });
}

void Game::wrapAroundScreen(glm::dvec2 &vTarget) {
    if (vTarget.x < 0) vTarget.x = mResolution.x;
    if (vTarget.x > mResolution.x) vTarget.x = 0;
    if (vTarget.y < 0) vTarget.y = mResolution.y;
    if (vTarget.y > mResolution.y) vTarget.y = 0;
}

void Game::generateAsteroids() {
    for (int i = 0; i < 10; i++) {
        Asteroid *a = mAsteroidPool.aquireObject();
        if (a) {
            double x = randRangeNaive(0, mResolution.x);
            double y = randRangeNaive(0, mResolution.y);
            randomizeAsteroid(a, 4, x, y);
        }
    }
}

void Game::randomizeAsteroid(Asteroid *a, int tier, double x, double y) {
        a->setPosition(x, y);
        double rot = randRangeNaive(0, twoPi);
        auto dirVec = glm::dvec2(glm::cos(rot), glm::sin(rot));
        a->dirNormal = glm::normalize(dirVec);
        a->regen(tier);
}

void Game::loadShaders() {
    plSh = new Shader("res/pl.vert.glsl", "res/pl.frag.glsl");
    passSh = new Shader("res/pass.vert.glsl", "res/pass.frag.glsl");
    blurSh = new Shader("res/blur.vert.glsl", "res/blur.frag.glsl");
    addSh = new Shader("res/add.vert.glsl", "res/add.frag.glsl");
    screenShakeSh = new Shader("res/screenshake.vert.glsl", "res/screenshake.frag.glsl");
    scanlineSh = new Shader("res/scanline.vert.glsl", "res/scanline.frag.glsl");
    colorShiftSh = new Shader("res/colorshift.vert.glsl", "res/colorshift.frag.glsl");
}

void Game::loadFilters() {
    passFilter = new PassthroughFilter(*passSh, mResolution);
    blurFilter = new BlurFilter(*blurSh, mResolution/2);
    blurFilter->setIterations(7);
    addFilter = new AddFilter(*addSh, mResolution);
    addFilter->setFactor(2);
    screenShakeFilter = new ScreenShakeFilter(*screenShakeSh, mResolution);
    scanlineFilter = new ScanlineFilter(*scanlineSh, mResolution);
    colorShiftFilter = new ColorShiftFilter(*colorShiftSh, mResolution);
}

void Game::unloadShaders() {
    if (plSh) delete plSh;
    if (passSh) delete passSh;
    if (blurSh) delete blurSh;
    if (addSh) delete  addSh;
    if (screenShakeSh) delete screenShakeSh;
}

void Game::unloadFilters() {
    if (passFilter) delete passFilter;
    if (blurFilter) delete blurFilter;
    if (addFilter) delete addFilter;
    if (screenShakeFilter) delete screenShakeFilter;
}

void Game::useAlphaBlending() {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
}

void Game::useAdditiveBlending() {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnable(GL_BLEND);
}

