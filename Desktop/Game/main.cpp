#include "game.h"

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 540;

int main(int argc, char* argv[]) {
    Game game(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!game.isRunning()) return -1;

    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;

    while (game.isRunning()) {
        frameStart = SDL_GetTicks();
        game.handleEvents();
        game.update();
        game.render();

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    return 0;
}
