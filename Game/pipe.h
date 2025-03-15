#pragma once



#include <SDL.h>
#include <SDL_image.h>

static const int PIPE_WIDTH = 65;
static const int PIPE_GAP = 200; // Khoảng cách giữa 2 ống
static const int PIPE_SPEED = 2;
static const int PIPE_HEIGHT = 373;
static const int PIPE_SPACING = 286;

class Pipe {
public:
    Pipe(SDL_Renderer* renderer, int startX);
    ~Pipe();
    int getgapY(); // Lấy vị trí Y của khoảng trống
    void update();
    void render(SDL_Renderer* renderer);
    int getX() { return x; }  // Lấy vị trí X để kiểm tra va chạm 

private:
    int x;  // Vị trí X của ống (di chuyển từ phải sang trái)
    int gapY;  // Vị trí khoảng trống giữa ống trên và dưới
    SDL_Texture* upPipeTexture;
    SDL_Texture* lowPipeTexture;
};


