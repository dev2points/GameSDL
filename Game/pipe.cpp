#include "Pipe.h"
#include <cstdlib>  // Để sử dụng rand()
#include <ctime>
#include <iostream>

double PIPE_SPEED = 3;
int PIPE_SPACING = 300;
int score = 0;

Pipe::Pipe(SDL_Renderer* renderer, int startX) {
    x = startX;
    gapY = rand() % 200 + 20; // Vị trí khoảng trống giữa ống trên và dưới

    upPipeTexture = IMG_LoadTexture(renderer, "assets/image/up_pipe.png");
    lowPipeTexture = IMG_LoadTexture(renderer, "assets/image/low_pipe.png");

    if (!upPipeTexture || !lowPipeTexture) {
        std::cout << "Lỗi load pipe: " << IMG_GetError();
    }
    //alpha = 0;
    isScored = false;
  
}

Pipe::~Pipe() {
    SDL_DestroyTexture(upPipeTexture);
    SDL_DestroyTexture(lowPipeTexture);
}

void Pipe::update(int last_x) {
    x -= (int) PIPE_SPEED;  // Ống di chuyển sang trái
	if (score > 0 && score % 47 == 0) alpha = true;
	
    // Nếu ống ra khỏi màn hình, reset lại vị trí
    if (x + PIPE_WIDTH < 0) {
        if (score > 0 && score % 48 == 0) x = last_x + PIPE_SPACING + 1000;  // Đưa ống về lại bên phải màn hình
        else x = last_x + PIPE_SPACING;
        gapY = rand() % 200 + 20;
        isScored = false;
    }
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

void Pipe::increase_Speed() {
    PIPE_SPEED += 2;
}
void Pipe::decrease_Speed() {
    PIPE_SPEED -= 2;
}
void Pipe::set_speed() {
    PIPE_SPEED = 3;
}


