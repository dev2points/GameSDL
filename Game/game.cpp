#include "game.h"
#include <iostream>

Game::Game(int screenWidth, int screenHeight) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "SDL Init Failed!\n";
        running = false;
        return;
    }

    window = SDL_CreateWindow("Flappy Doge", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cout << "Window Creation Failed!\n";
        running = false;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cout << "Renderer Creation Failed!\n";
        running = false;
        return;
    }

    running = true;

    // Khởi tạo Background, Doge và Land
    background = new Background(renderer, "assets/image/background_2.jpg", screenWidth, BACKGROUND_HEIGHT + 7);
    doge = new Doge(renderer, "assets/image/shiba.png", 450, 50);
    pipe = new Pipe(renderer,800);
    land = new Land(renderer, "assets/image/land_2.jpg", screenWidth, BACKGROUND_HEIGHT);
}

Game::~Game() {
    delete background;
    delete doge;
    delete pipe;
    delete land;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool Game::isRunning() const {
    return running;
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        if (event.type == SDL_MOUSEBUTTONDOWN ||
            (event.type == SDL_KEYDOWN && (event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_UP))) {
            if (doge) {
                doge->jump();
            }
        }
    }
}

void Game::update() {
    background->update();
    doge->update();
    //land->update();
}

void Game::render() {
    SDL_RenderClear(renderer);
    background->render(renderer);
    doge->render(renderer);
    pipe->render(renderer);
    land->render(renderer);
    SDL_RenderPresent(renderer);
}
