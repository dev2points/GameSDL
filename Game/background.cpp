

#include "background.h"

Background::Background(SDL_Renderer* renderer, const char* filePath, int width, int height, bool center) {
    texture = IMG_LoadTexture(renderer, filePath);
    if (!texture) {
        std::cout << "Lỗi tải ảnh: " << IMG_GetError() << std::endl;
        return;
    }
    Center = center;
    screenWidth = width;
    screenHeight = height;
    scrollX = 0;
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);// Lấy kích thước ảnh

    dest.x = width ;
    dest.y = height;
    src1 = { -scrollX, 0, dest.w + scrollX, dest.h };
    src2 = { 0, 0, -scrollX, dest.h };
    dest1 = { 0, 0, screenWidth, screenHeight };
    dest2 = { screenWidth, 0, screenWidth, screenHeight };


}

Background::~Background() {
    SDL_DestroyTexture(texture);
}

void Background::update() {
	scrollX -= 1; // Cuộn qua trái
    if (scrollX <= -dest.w) {
        scrollX = 0;
    }

    src1 = { -scrollX, 0, dest.w + scrollX, dest.h };
    dest1 = { 0, 0, screenWidth + scrollX * screenWidth / dest.w, screenHeight };

    src2 = { 0, 0, -scrollX, dest.h };
    dest2 = { screenWidth + scrollX * screenWidth / dest.w, 0, -scrollX * screenWidth / dest.w, screenHeight };
}
void Background::effect_update() {
    scrollY += 3;  // Cuộn xuống

    if (scrollY >= dest.h) {
        scrollY = 0;
    }

    src1 = { 0, 0, dest.w, dest.h - scrollY };
    dest1 = { 0, scrollY, screenWidth, screenHeight - scrollY * screenHeight / dest.h };

    src2 = { 0, dest.h - scrollY, dest.w, scrollY };
    dest2 = { 0, 0, screenWidth, scrollY * screenHeight / dest.h };
}


void Background::render(SDL_Renderer* renderer) {
    if (!Center) {
        SDL_RenderCopy(renderer, texture, &src1, &dest1);
        SDL_RenderCopy(renderer, texture, &src2, &dest2);
    }
    else {
        SDL_RenderCopy(renderer, texture, NULL, &dest);
    }

}

SDL_Texture* Background::getTexture() {
    return texture;
}

int Background::get_x() {
    return dest.x;
}
int Background::get_y() {
    return dest.y;
}
