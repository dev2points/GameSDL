#include "Pipe.h"
#include <cstdlib>  // Để sử dụng rand()
#include <ctime>
#include <iostream>

Pipe::Pipe(SDL_Renderer* renderer, int startX) {
    x = startX;
    gapY = rand() % 300 + 20; // Vị trí khoảng trống giữa ống trên và dưới


    upPipeTexture = IMG_LoadTexture(renderer, "assets/image/up_pipe.png");
    lowPipeTexture = IMG_LoadTexture(renderer, "assets/image/low_pipe.png");

    if (!upPipeTexture || !lowPipeTexture) {
        std::cout<<"Lỗi load pipe: "<< IMG_GetError();
    }
}

Pipe::~Pipe() {
    SDL_DestroyTexture(upPipeTexture);
    SDL_DestroyTexture(lowPipeTexture);
}

void Pipe::update() {
    x -= PIPE_SPEED;  // Ống di chuyển sang trái

    // Nếu ống ra khỏi màn hình, reset lại vị trí
    if (x + PIPE_WIDTH < 0) {
        x = 1080;  // Đưa ống về lại bên phải màn hình
        gapY = rand() % 300 + 20;

    }
}

void Pipe::render(SDL_Renderer* renderer) {
    SDL_Rect upPipesrc = {0, PIPE_HEIGHT - gapY, PIPE_WIDTH, gapY };
    SDL_Rect lowPipesrc = {0, 0, PIPE_WIDTH,  600 - gapY - PIPE_GAP};

    SDL_Rect upPipedest = { x, 0 , PIPE_WIDTH, gapY};
    SDL_Rect lowPipedest = { x, gapY + PIPE_GAP, PIPE_WIDTH,  600 - gapY - PIPE_GAP };

    SDL_RenderCopy(renderer, upPipeTexture, &upPipesrc, &upPipedest);
    SDL_RenderCopy(renderer, lowPipeTexture, &lowPipesrc, &lowPipedest);
}
