#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

class Doge {
private:
    SDL_Texture* texture;
    SDL_Rect src, dest;
    int x, y,velocit;
    int velocity = 0;   // Vận tốc rơi
    int gravity = 1;  // Trọng lực
    int jumpForce = -8; // Lực nhảy


public:
    Doge(SDL_Renderer* renderer, const char* filePath, int startX, int startY);
    ~Doge();

    void update();
    void render(SDL_Renderer* renderer);
};


