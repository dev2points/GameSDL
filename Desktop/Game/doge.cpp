
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



void Doge::update() {

    velocity += gravity;
    dest.y += velocity;
    velocity = gravity;

}

void Doge::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, texture, &src, &dest);
}
