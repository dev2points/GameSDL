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
    play = false; // Kiểm tra trạng thái bắt đầu chơi
    running = true; //  Trạng thái bắt đầu chạy

    // Khởi tạo Background, Doge và Land
    background = new Background(renderer, "assets/image/background_2.jpg", screenWidth, BACKGROUND_HEIGHT + 7, 1);
    message = new Background(renderer, "assets/image/message.png", (screenWidth - MESSAGE_WIDTH) / 2, (screenHeight - MESSAGE_HEIGHT) / 2, 0);
    doge = new Doge(renderer, "assets/image/shiba.png", 507, 285);

    // Khởi tạo 4 ống với khoảng cách nhau
    for (int i = 0; i < 4; i++) {
        pipes.push_back(new Pipe(renderer, 1080 + i * PIPE_SPACING));
    }

    land = new Land(renderer, "assets/image/land_2.jpg", screenWidth, BACKGROUND_HEIGHT);
}

Game::~Game() {
    delete background;
    delete doge;
    for (auto& pipe : pipes) {
    delete pipe;
    }
    delete land;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
bool Game::isPlay() {
    return play;
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
            play = true;
            if (doge) {
                doge->jump();
            }
        }
    }
}

void Game::update() {
    background->update();
    doge->update();
    for (auto& pipe : pipes) {
    pipe->update();
    }
    land->update();
}

void Game::render() {
    SDL_RenderClear(renderer);
    background->render(renderer);
    doge->render(renderer);
    if(!play) message->render(renderer);
    for (auto& pipe : pipes) {
    pipe->render(renderer);
    }
    land->render(renderer);
    SDL_RenderPresent(renderer);
}
