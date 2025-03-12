#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

static const int BACKGROUND_HEIGHT =490;
static const int MESSAGE_HEIGHT =204;
static const int MESSAGE_WIDTH =225;



class Background {
private:
    SDL_Texture* texture;
    SDL_Rect src1, dest1, src2, dest2, dest;
    int scrollX;
    int screenWidth, screenHeight;
    bool Center;

public:

    Background(SDL_Renderer* renderer, const char* filePath, int width, int height, bool center);
    ~Background();

    void update();
    void render(SDL_Renderer* renderer);
};



