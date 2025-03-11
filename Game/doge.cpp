
#include "doge.h"


Doge::Doge(SDL_Renderer* renderer, const char* filePath, int startX, int startY) {
    SDL_Surface* tempSurface = IMG_Load(filePath);
    if (!tempSurface) {
        std::cout << "Lỗi tải ảnh Doge: " << IMG_GetError() << std::endl;
        texture = nullptr;
        return;
    }
    texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    x = startX;
    y = startY;
    src = {0, 0, 0, 0};
    SDL_QueryTexture(texture, NULL, NULL, &src.w, &src.h);

    dest = {x, y, src.w, src.h};
}

Doge::~Doge() {
    SDL_DestroyTexture(texture);
}

void Doge::jump() {
    velocity = jumpForce;
    angle = -20.0;
}


void Doge::update() {
    //trọng lực rơi
    velocity += gravity;
    if (velocity > maxFallSpeed) velocity = maxFallSpeed; // giới hạn vận tốc
    dest.y += velocity;

   //xoay khi rơi
    if (velocity > 0) {
        angle += 1.5; //
        if (angle > 90) angle = 90; // không xoay quá 90 độ
    }
}



void Doge::render(SDL_Renderer* renderer) {
    SDL_RenderCopyEx(renderer, texture, &src, &dest, angle, nullptr, SDL_FLIP_NONE);
}


