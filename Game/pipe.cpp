﻿#include "Pipe.h"
#include <cstdlib>  // Để sử dụng rand()
#include <ctime>
#include <iostream>

int Pipe::PIPE_SPEED = 4;

Pipe::Pipe(SDL_Renderer* renderer, int startX) {
    x = startX;
    gapY = rand() % 230 + 20; // Vị trí khoảng trống giữa ống trên và dưới

    upPipeTexture = IMG_LoadTexture(renderer, "assets/image/up_pipe.png");
    lowPipeTexture = IMG_LoadTexture(renderer, "assets/image/low_pipe.png");

    if (!upPipeTexture || !lowPipeTexture) {
        std::cout << "Lỗi load pipe: " << IMG_GetError();
    }
    alpha = 0;
    isScored = false;
  
}

Pipe::~Pipe() {
    SDL_DestroyTexture(upPipeTexture);
    SDL_DestroyTexture(lowPipeTexture);
}

void Pipe::update() {
    x -= PIPE_SPEED;  // Ống di chuyển sang trái

    // Nếu ống ra khỏi màn hình, reset lại vị trí
    if (x + PIPE_WIDTH < 0 && !alpha) {
        x = 1080;  // Đưa ống về lại bên phải màn hình
        gapY = rand() % 300 + 20;
        isScored = false;
    }
    //std::cout << PIPE_SPEED << std::endl;
}

int Pipe::getgapY() {
    return gapY;
}

void Pipe::render(SDL_Renderer* renderer) {
    SDL_Rect upPipesrc = { 0, PIPE_HEIGHT - gapY, PIPE_WIDTH, gapY };
    SDL_Rect lowPipesrc = { 0, 0, PIPE_WIDTH,  600 - gapY - PIPE_GAP };

    SDL_Rect upPipedest = { x, 0 , PIPE_WIDTH, gapY };
    SDL_Rect lowPipedest = { x, gapY + PIPE_GAP, PIPE_WIDTH,  600 - gapY - PIPE_GAP };

    SDL_RenderCopy(renderer, upPipeTexture, &upPipesrc, &upPipedest);
    SDL_RenderCopy(renderer, lowPipeTexture, &lowPipesrc, &lowPipedest);
}

void Pipe::check_win() {
    alpha = 1;
}

void Pipe::increase_Speed() {
    PIPE_SPEED += 2;
}
void Pipe::decrease_Speed() {
    PIPE_SPEED -= 2;
}
void Pipe::set_speed() {
    PIPE_SPEED = 3;
}

