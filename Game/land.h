#pragma once

#include "lib.h"
#include <SDL.h>
#include <SDL_image.h>

class Land{
private:
    SDL_Texture* texture;
    SDL_Rect src1, dest1, src2, dest2, dest;
    int scrollX, counter = 0;
    int Width, Height;


public:
    Land(SDL_Renderer* renderer, const char* filePath, int width, int height);
    ~Land();

    void update();
    void render(SDL_Renderer* renderer);


};
