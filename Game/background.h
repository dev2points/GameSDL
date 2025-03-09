#pragma once


#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

class Background {
private:
    SDL_Texture* texture;
    SDL_Rect src1, dest1, src2, dest2, dest;
    int scrollX, counter = 0;
    int screenWidth, screenHeight;

public:
    Background(SDL_Renderer* renderer, const char* filePath, int width, int height);
    ~Background();

    void update();
    void render(SDL_Renderer* renderer);
};



