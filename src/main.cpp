#include "Game.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <cstdlib>
#include <ctime>

using namespace std;

int main(int argc, char *argv[]) {
    srand((unsigned) time(NULL));
    Game game;
    game.mainloop();

    return 0;
}