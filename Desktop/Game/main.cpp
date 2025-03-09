#include "game.h"
#include "background.h"
#include "doge.h"

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 540;

int main(int argc, char* argv[]) {
    Game game(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!game.isRunning()) return -1;

    Background background(game.getRenderer(), "assets/image/background_1.jpg", SCREEN_WIDTH, SCREEN_HEIGHT);
    Doge doge(game.getRenderer(),"assets/image/shiba.png",450,50);

    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;

    while (game.isRunning()) {
        frameStart = SDL_GetTicks();
        game.handleEvents();
        background.update();
        game.render();
        background.render(game.getRenderer());
        doge.render(game.getRenderer());
        SDL_RenderPresent(game.getRenderer());




        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    return 0;
}
