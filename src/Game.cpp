#include "Game.h"
#include "Polyline.h"
#include "PolylineRenderer.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/GL.h>

const int WIN_W = 800;
const int WIN_H = 600;

Game::Game()
        : mDone(false) {

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

    mWin = SDL_CreateWindow("Asteroids", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, WIN_W, WIN_H, SDL_WINDOW_OPENGL);

    mContext = SDL_GL_CreateContext(mWin);
    SDL_GL_SetSwapInterval(0);		// vsync 0:off - 1:on

    glewInit();

    glEnable(GL_DEBUG_OUTPUT);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(0, 0, WIN_W, WIN_H);
}

Game::~Game() {
    SDL_GL_DeleteContext(mContext);
    SDL_DestroyWindow(mWin);
}

void Game::mainloop() {
    Shader sh("../res/pl.vert.glsl", "../res/pl.frag.glsl");
    Polyline pl;
    pl.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    pl.width = 4;
    pl.PivotOn(Polyline::PivotCG);
    pl.addPoint(100, 100);
    pl.addPoint(200, 100);
    pl.addPoint(150, 150);
    pl.addPoint(100, 100);
    pl.closed = true;

    PolylineRenderer plr(sh, glm::vec2(WIN_W, WIN_H));

    while (!mDone) {
        handleInput();

        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        plr.begin();
        //plr.render(pl);
        pl.render(plr);
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
            } else if (e.key.keysym.sym == SDLK_SPACE) {
            }
        }
    }
}

