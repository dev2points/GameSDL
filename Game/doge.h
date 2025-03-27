#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "background.h"

const static int BIRD_HEIGHT = 50;
const static int BIRD_WIDTH = 90;

const static int PAUSE_WIDTH = 26;
const static int PAUSE_HEIGHT = 28;
static double velocity;  // vận tốc

class Doge {
private:
    SDL_Texture* texture;
    SDL_Rect dest, src;
    int x, y, time;
    int currentFrame = 0;
    Uint32 lastUpdate;
    int frameDelay = 100;
    std::string filepath;
    
    double gravity = 0.15; // trọng lực
    double maxFallSpeed = 8.0;
    double angle = 0.0;

    //double championScale = 0.1;  // Bắt đầu champion với kích thước nhỏ
    //bool growing = true;         // Kiểm soát trạng thái phóng to champion

public:

    int getX();
    int getY();
    void set_x(int x);
    void set_y(int y);

    SDL_Rect getRect();
    std::string getFilePath();
    Doge(SDL_Renderer* renderer, const char* filePath, int startX, int startY, bool a);
    ~Doge();
    void jump();
    void increase_velocity();
    void decrease_velocity();
    void update();
    void render_bird(SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer);
    void update_src();
    void time_playing(Uint32 time) { Uint32 lastUpdate = time; };

    void update_powerup();
};


