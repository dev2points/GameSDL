#include "game.h"
#include "doge.h"
using namespace std;

Game::Game(int width, int height) : screenWidth(width), screenHeight(height), running(true) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        cout << "Lỗi khởi tạo SDL: " << SDL_GetError() << endl;
        running = false;
        return;
    }

    window = SDL_CreateWindow("Flappy Doge", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    if (!window) {
        cout << "Lỗi cửa sổ: " << SDL_GetError() << endl;
        running = false;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cout << "Lỗi render: " << SDL_GetError() << endl;
        running = false;
        return;
    }

    if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) & (IMG_INIT_PNG | IMG_INIT_JPG))) {
        cout << "Lỗi khởi tạo SDL_image: " << IMG_GetError() << endl;
        running = false;
        return;
    }
}

Game::~Game() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
    }
}

void Game::update() {}

void Game::render() {
    SDL_RenderClear(renderer);
}

bool Game::isRunning() const {
    return running;
}

SDL_Renderer* Game::getRenderer() const {
    return renderer;
}

