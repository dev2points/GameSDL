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
    std::string filepath;

    double velocity ;  // vận tốc
    double gravity = 0.15; // trọng lực
    double maxFallSpeed = 8.0;
    double angle = 0.0;

    double championScale = 0.1;  // Bắt đầu champion với kích thước nhỏ
    bool growing = true;         // Kiểm soát trạng thái phóng to champion



public:

    int getX();
    int getY();
    SDL_Rect getRect();
    std::string getFilePath();
    Doge(SDL_Renderer* renderer, const char* filePath, int startX, int startY);
    ~Doge();
    void jump();
    void increase_velocity();
    void decrease_velocity();
    void update();
    void render(SDL_Renderer* renderer);

    void updateChampion();

    void update_powerup();
};


