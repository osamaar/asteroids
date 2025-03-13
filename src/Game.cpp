#include "Game.h"
#include "Ship.h"
#include "Asteroid.h"
#include "Bullet.h"
#include "Polyline.h"
#include "PolylineRenderer.h"
#include "Shader.h"
#include "helpers.h"
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/GL.h>

const int WIN_W = 800;
const int WIN_H = 600;

Game::Game()
        : mDone(false)
        , mThrusting(false)
        , mShooting(false)
        , mRotating(0)
        , mAsteroidPool(100)
        , mPlayerBulletPool(400) {

    SDL_Init(SDL_INIT_EVERYTHING);

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

    mWin = SDL_CreateWindow("Asteroids", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, WIN_W, WIN_H, SDL_WINDOW_OPENGL);

    mContext = SDL_GL_CreateContext(mWin);
    SDL_GL_SetSwapInterval(1);		// vsync 0:off - 1:on

    glewInit();

    glEnable(GL_DEBUG_OUTPUT);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glEnable(GL_MULTISAMPLE);

    glViewport(0, 0, WIN_W, WIN_H);
}

Game::~Game() {
    SDL_GL_DeleteContext(mContext);
    SDL_DestroyWindow(mWin);
}

void Game::mainloop() {
    Shader sh("../../res/pl.vert.glsl", "../../res/pl.frag.glsl");

    PolylineRenderer plr(sh, glm::vec2(WIN_W, WIN_H));

    Ship ship;
    ship.setPosition(WIN_W/2, WIN_H/2);

    generateAsteroids();

    while (!mDone) {
        // Init.
        //mThrusting = false;
        //mShooting = false;
        //mRotating = 0;

        // Input.
        handleInput();

        // Update.
        updateShip(ship);
        updatePlayerBullets();
        updateEnemyBullets();
        updateAsteroids();

        // Draw.
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);

        plr.begin();
        ship.render(plr);
        //asteroid->render(plr);

        mAsteroidPool.apply([&](Asteroid& a) {
            if (a.poolState.alive) {
                a.render(plr);
            }
        });

        mPlayerBulletPool.apply([&](Bullet& b) {
            if (b.poolState.alive) {
                b.render(plr);
            }
        });

        plr.end();

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
            } else if (e.key.keysym.sym == SDLK_UP) {
                mThrusting = true;
            } else if (e.key.keysym.sym == SDLK_LEFT) {
                mRotating = -1;
            } else if (e.key.keysym.sym == SDLK_RIGHT) {
                mRotating = 1;
            } else if (e.key.keysym.sym == SDLK_SPACE) {
                mShooting = true;
            }
        } else if (e.type == SDL_KEYUP) {
            if (e.key.keysym.sym == SDLK_UP) {
                mThrusting = false;
            } else if (e.key.keysym.sym == SDLK_LEFT) {
                mRotating = 0;
            } else if (e.key.keysym.sym == SDLK_RIGHT) {
                mRotating = 0;
            } else if (e.key.keysym.sym == SDLK_SPACE) {
                mShooting = false;
            }
        }
    }
}

void Game::updateShip(Ship &ship) {
    if (mThrusting) {
        ship.thrust(true);
    } else {
        ship.thrust(false);
    }

    if (mShooting) {
        ship.shoot(mPlayerBulletPool);
    }

    ship.rotate(mRotating);
    auto shipPos = ship.getPosition();
    wrapAroundScreen(shipPos);
    ship.setPosition(shipPos.x, shipPos.y);
    ship.update();
}

void Game::updatePlayerBullets() {
    mPlayerBulletPool.apply([&](Bullet& b) {
        b.update();
        auto bPos = b.getPosition();
        wrapAroundScreen(bPos);
        b.setPosition(bPos.x, bPos.y);

        mAsteroidPool.apply([&](Asteroid& a) {
            if (collides(a, b)) {
                a.poolState.alive = false;
                b.poolState.alive = false;
            }
        });
    });
}

void Game::updateEnemyBullets() {

}

void Game::updateAsteroids() {
    mAsteroidPool.apply([&](Asteroid& a) {
        a.update();
        auto aPos = a.getPosition();
        wrapAroundScreen(aPos);
        a.setPosition(aPos.x, aPos.y);
    });

}

void Game::wrapAroundScreen(glm::dvec2 &vTarget) {
    if (vTarget.x < 0) vTarget.x = WIN_W;
    if (vTarget.x > WIN_W) vTarget.x = 0;
    if (vTarget.y < 0) vTarget.y = WIN_H;
    if (vTarget.y > WIN_H) vTarget.y = 0;
}

void Game::generateAsteroids() {
    for (int i = 0; i < 10; i++) {
        Asteroid *a = mAsteroidPool.aquireObject();
        if (a) {
            double x = randRangeNaive(0, WIN_W);
            double y = randRangeNaive(0, WIN_H);
            a->setPosition(x, y);
            double rot = randRangeNaive(0, twoPi);
            auto dirVec = glm::dvec2(glm::cos(rot), glm::sin(rot));
            a->dirNormal = glm::normalize(dirVec);
            a->regenShape(4);
        }
    }
}

