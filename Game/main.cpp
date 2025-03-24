
#include "game.h"
#include "background.h"

static const int SCREEN_WIDTH = 1000;
static const int SCREEN_HEIGHT = 600;


int main(int argc, char* argv[]) {
    Game game(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!game.isRunning()) return -1;

    int FPS = 60;
    int frameDelay = 1000 / FPS;  
    Uint32 frameStart;
    int frameTime;


    while (game.isRunning()) {
        frameStart = SDL_GetTicks();
        game.handleEvents();
        if (game.isPlay() ) game.update();
        game.check();
        game.render();
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    return 0;
}
