#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

class Game {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
    const int screenWidth;
    const int screenHeight;

public:
    Game(int width, int height);
    ~Game();
    void handleEvents();
    void update();
    void render();
    bool isRunning() const;
    SDL_Renderer* getRenderer() const;
};


