#pragma once



#include <SDL.h>
#include <SDL_image.h>

static const int PIPE_WIDTH = 65;
static const int PIPE_GAP = 200; // Khoảng cách giữa 2 ống
extern double PIPE_SPEED;
static const int PIPE_HEIGHT = 373;
extern int PIPE_SPACING ;

class Pipe {
public:
    Pipe(SDL_Renderer* renderer, int startX);
    ~Pipe();
    int getgapY(); // Lấy vị trí Y của khoảng trống
    void update(int last_x);
    void render(SDL_Renderer* renderer);
    int getX() { return x; }  // Lấy vị trí X để kiểm tra va chạm 
    bool isScored;
    static void increase_Speed();
    static void decrease_Speed();
    static void set_speed();
    

private:
    int x;  // Vị trí X của ống (di chuyển từ phải sang trái)
    int gapY;  // Vị trí khoảng trống giữa ống trên và dưới
    SDL_Texture* upPipeTexture;
    SDL_Texture* lowPipeTexture;
    bool alpha;
    
};


