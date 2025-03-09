#pragma once
#include <SDL.h>
#include "background.h"
#include "doge.h"

class Game {
public:
    Game(int screenWidth, int screenHeight);
    ~Game();

    bool isRunning() const;
    void handleEvents();
    void update();
    void render();
    SDL_Renderer* getRenderer() { return renderer; }

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;

    Background* background;
    Doge* doge;
};
