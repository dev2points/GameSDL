#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

using namespace std;

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 540;

int main(int argc, char* argv[]) {

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        cout << "Lỗi khởi tạo SDL: " << SDL_GetError() << endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        cout << "Lỗi cửa sổ: " << SDL_GetError() << endl;
        SDL_Quit();
        return -2;
    }

    SDL_Renderer* render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!render) {
        cout << "Lỗi render: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -3;
    }

if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) & (IMG_INIT_PNG | IMG_INIT_JPG))) {
    cout << "Lỗi khởi tạo SDL_image: " << IMG_GetError() << endl;
    SDL_Quit();
    return -2;
}

    SDL_Texture* background_texture = IMG_LoadTexture(render, "assets/image/background_1.jpg");
    SDL_Texture* shiba_texture = IMG_LoadTexture(render, "assets/image/shiba.png");

    if (!background_texture ) {
        cout << "Lỗi tải ảnh: " << IMG_GetError() << endl;
        SDL_DestroyRenderer(render);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -4;
    }

    // Lấy kích thước ảnh nền
    SDL_Rect background_dest;
    SDL_QueryTexture(background_texture, NULL, NULL, &background_dest.w, &background_dest.h);
    background_dest.x = 0;
    background_dest.y = 0;

    SDL_Rect shiba_src;
    SDL_QueryTexture(shiba_texture, NULL, NULL, &shiba_src.w, &shiba_src.h);
    shiba_src.x = 0;
    shiba_src.y = 0;
    // Lấy kích thước ảnh mặt đất

    // Biến điều khiển cuộn nền
    int scrollX = 0; // Vị trí cuộn ngang
    bool running = true;
    SDL_Event event;
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        frameStart = SDL_GetTicks();
        // Dịch chuyển nền từ phải sang trái
        scrollX -= 2;
        if (scrollX <= -background_dest.w) scrollX = 0;

        // Vẽ nền (2 ảnh nền nối tiếp nhau để tạo hiệu ứng cuộn)
        SDL_Rect src1 = { -scrollX, 0, background_dest.w+scrollX, background_dest.h };
        SDL_Rect dest1 = { 0, 0, SCREEN_WIDTH+scrollX*3/2, SCREEN_HEIGHT };

        SDL_Rect shiba_dest = { 400, 250, shiba_src.w, shiba_src.h };

        SDL_Rect src2 = { 0, 0,-scrollX, background_dest.h};
        SDL_Rect dest2 = { SCREEN_WIDTH + scrollX*3/2, 0, -scrollX*3/2, SCREEN_HEIGHT};

        SDL_RenderClear(render);
        SDL_RenderCopy(render, background_texture, &src1, &dest1);
        SDL_RenderCopy(render, background_texture, &src2, &dest2);
        SDL_RenderCopy(render, shiba_texture, NULL, &shiba_dest);




        SDL_RenderPresent(render);


        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        } // Giới hạn tốc độ FPS (~60 FPS)

    }



    // Dọn dẹp tài nguyên
    SDL_DestroyTexture(background_texture);

    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
