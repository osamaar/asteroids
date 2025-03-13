#pragma once


#include <SDL2/SDL.h>

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
};

