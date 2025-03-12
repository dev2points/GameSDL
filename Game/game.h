#pragma once

#include <SDL.h>
#include <vector>
#include "background.h"
#include "doge.h"
#include "pipe.h"
#include "land.h"

class Game {
public:
    Game(int screenWidth, int screenHeight);
    ~Game();

    bool isRunning() const;
    bool isPlay() ;
    void handleEvents();
    void update();
    void render();
    SDL_Renderer* getRenderer() { return renderer; }

private:

    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running,play;

    Background* background;
    Background* message;
    Doge* doge;
    std::vector<Pipe*> pipes;
    Pipe* pipe;
    Land* land;
};
