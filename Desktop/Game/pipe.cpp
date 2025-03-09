#include "Pipe.h"
#include <cstdlib>  // Để sử dụng rand()
#include <ctime>
#include <iostream>   // Để random vị trí ống

Pipe::Pipe(SDL_Renderer* renderer, int startX) {
    x = startX;
    gapY = rand() % 200 + 100; // Random khoảng trống giữa ống


    upPipeTexture = IMG_LoadTexture(renderer, "assets/up_pipe.png");
    lowPipeTexture = IMG_LoadTexture(renderer, "assets/low_pipe.png");

    if (!upPipeTexture || !lowPipeTexture) {
        std::cout<<("Lỗi load pipe: %s\n", IMG_GetError());
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
        x = 800;  // Đưa ống về lại bên phải màn hình
        gapY = rand() % 200 + 100; // Random vị trí mới
    }
}

void Pipe::render(SDL_Renderer* renderer) {
    SDL_Rect upPipeRect = { x, gapY - 320, PIPE_WIDTH, 320 };
    SDL_Rect lowPipeRect = { x, gapY + PIPE_GAP, PIPE_WIDTH, 320 };

    SDL_RenderCopy(renderer, upPipeTexture, NULL, &upPipeRect);
    SDL_RenderCopy(renderer, lowPipeTexture, NULL, &lowPipeRect);
}
