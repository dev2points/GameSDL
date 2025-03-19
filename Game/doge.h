#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "background.h"

const static int DOGE_HEIGHT = 35;
const static int DOGE_WIDTH = 50;



class Doge {
private:
    SDL_Texture* texture;
    SDL_Rect dest;
    int x, y, time;
    double velocity = 0.0;  // vận tốc
    double gravity = 0.15; // trọng lực
    double maxFallSpeed = 8.0;
    double angle = 0.0;

    double championScale = 0.1;  // Bắt đầu với kích thước nhỏ
    bool growing = true;         // Kiểm soát trạng thái phóng to



public:
    static const int PIPE_WIDTH = 65;
    static const int PIPE_GAP = 150; // Khoảng cách giữa 2 ống
    static const int PIPE_SPEED = 2;
    static const int PIPE_HEIGHT = 373;

    int getX();
    int getY();
    Doge(SDL_Renderer* renderer, const char* filePath, int startX, int startY);
    ~Doge();
    void jump();
    void update();
    void render(SDL_Renderer* renderer);

    void updateChampion();
};


