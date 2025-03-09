

#include "background.h"

Background::Background(SDL_Renderer* renderer, const char* filePath, int width, int height) {
    texture = IMG_LoadTexture(renderer, filePath);
    if (!texture) {
        std::cout << "Lỗi tải ảnh: " << IMG_GetError() << std::endl;
        return;
    }
    screenWidth = width;
    screenHeight = height;
    scrollX = 0;
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    dest1 = {0, 0, screenWidth, screenHeight};
    dest2 = {screenWidth, 0, screenWidth, screenHeight};
}

Background::~Background() {
    SDL_DestroyTexture(texture);
}

void Background::update() {
    scrollX -= 1;
    if (scrollX <= -dest.w){
     counter ++;
     scrollX = 0;
    }

    src1 = { -scrollX, 0, screenWidth + scrollX, screenHeight };
    dest1 = { 0, 0, screenWidth + scrollX * screenWidth /dest.w, screenHeight };

    src2 = { 0, 0, -scrollX, screenHeight };
    dest2 = { screenWidth + scrollX * screenWidth /dest.w, 0, -scrollX * screenWidth /dest.w, screenHeight };
}

void Background::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, texture, &src1, &dest1);
    SDL_RenderCopy(renderer, texture, &src2, &dest2);
}
