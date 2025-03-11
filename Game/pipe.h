#pragma once

#include "lib.h"
#include <SDL.h>
#include <SDL_image.h>

class Pipe {
public:
    Pipe(SDL_Renderer* renderer, int startX);
    ~Pipe();

    void update();
    void render(SDL_Renderer* renderer);
    int getX() { return x; }  // Lấy vị trí X để kiểm tra va chạm sau này

private:
    int x;  // Vị trí X của ống (di chuyển từ phải sang trái)
    int gapY;  // Vị trí khoảng trống giữa ống trên và dưới
    SDL_Texture* upPipeTexture;
    SDL_Texture* lowPipeTexture;
};


