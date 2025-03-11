#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>




class Doge {
private:
    SDL_Texture* texture;
    SDL_Rect src, dest;
    int x, y,velocit;
    int jumpForce = -10;
    double gravity = 1;
    double velocity = 0.0;
    double maxFallSpeed = 8.0;
    double angle = 0.0;



public:
    static const int PIPE_WIDTH = 65;
    static const int PIPE_GAP = 150; // Khoảng cách giữa 2 ống
    static const int PIPE_SPEED = 2;
    static const int PIPE_HEIGHT = 373;

    Doge(SDL_Renderer* renderer, const char* filePath, int startX, int startY);
    ~Doge();
    void jump();
    void update();
    void render(SDL_Renderer* renderer);
};


