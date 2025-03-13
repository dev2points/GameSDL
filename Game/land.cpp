
#include "land.h"
#include <iostream>

Land::Land(SDL_Renderer* renderer, const char* filePath, int width, int height) {
    texture = IMG_LoadTexture(renderer, filePath);
    if (!texture) {
        std::cout << "Lỗi tải ảnh: " << IMG_GetError() << std::endl;
        return;
    }
    screenWidth = width;
    screenHeight = 600 - height;
    y = height;
    scrollX = 0;
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    src1 = { -scrollX, 0, dest.w + scrollX, dest.h };
    src2 = { 0, 0, -scrollX, dest.h };

    dest1 = { 0, y, screenWidth, screenHeight };
    dest2 = { screenWidth, y, screenWidth, screenHeight };
}

Land::~Land() {
    SDL_DestroyTexture(texture);
}

void Land::update() {
    scrollX -= 3;
    if (scrollX <= -dest.w) {
        counter++;
        scrollX = 0;
    }

    src1 = { -scrollX, 0, screenWidth + scrollX, screenHeight };
    dest1 = { 0, y, screenWidth + scrollX * screenWidth / dest.w, screenHeight };

    src2 = { 0, 0, -scrollX, screenHeight };
    dest2 = { screenWidth + scrollX * screenWidth / dest.w, y, -scrollX * screenWidth / dest.w, screenHeight };
}

void Land::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, texture, &src1, &dest1);
    SDL_RenderCopy(renderer, texture, &src2, &dest2);
}
