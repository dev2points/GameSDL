#include "game.h"
#include <iostream>
#include <string>
#include<fstream>


Game::Game(int screenWidth, int screenHeight) {

    screenwidth = screenWidth;
    screenheight = screenHeight;

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

    // Khởi tạo sound
    sound = new Sound();
    sound_end = false;
    sound_playing = false;

    score = 95;     
        
    play = false; // Kiểm tra trạng thái bắt đầu chơi
    running = true; //  Trạng thái bắt đầu chạy chương trình
    lose = false; //    Thua game
    win = false; // Thắng game

    // Khởi tạo Background, Doge và Land
    // Khởi tạo pointer để tồn tại xuyên suốt đến khi delete
    background_1 = new Background(renderer, "assets/image/background_1.jpg", screenWidth, BACKGROUND_HEIGHT + 7, 0);
    background_2 = new Background(renderer, "assets/image/background_2.jpg", screenWidth, BACKGROUND_HEIGHT + 7, 0);
    background_3 = new Background(renderer, "assets/image/background_3.jpg", screenWidth, BACKGROUND_HEIGHT + 7, 0);
    message = new Background(renderer, "assets/image/message.png", (screenWidth - MESSAGE_WIDTH) / 2, (screenHeight - MESSAGE_HEIGHT) / 2, 1);
    doge = new Doge(renderer, "assets/image/shiba.png", 429, 285); // Tọa độ X phải lẻ để hàm check hoạt động
    
    game_over = new Background(renderer, "assets/image/gameOver.png", (screenWidth - 250) / 2, (screenHeight - 209) / 2 -50, 1);
    replay = new Background(renderer, "assets/image/replay.png", (screenWidth - 100) / 2, (screenHeight - 56) / 2 + 100 , 1);

    // Khởi tạo 4 ống với khoảng cách nhau
    for (int i = 0; i < 4; i++) {
        pipes.push_back(new Pipe(renderer, 1080 + i * PIPE_SPACING));
    }

    land_1 = new Land(renderer, "assets/image/land_1.jpg", screenWidth, BACKGROUND_HEIGHT);
    land_2 = new Land(renderer, "assets/image/land_2.jpg", screenWidth, BACKGROUND_HEIGHT);
    land_3 = new Land(renderer, "assets/image/land_3.jpg", screenWidth, BACKGROUND_HEIGHT);

    currentBg = background_1;
    currentLand = land_1;
    nextBg = nullptr;
    nextLand = nullptr;

    Mix_PlayMusic(sound->waiting, -1);
    Mix_VolumeMusic(20);

    // Khởi tạo chữ số lớn hiển thị score
    for (int i = 0; i < 10; i++) {
        std::string filePath = "assets/image/large/" + std::to_string(i) + ".png"; 
        units_large_one_digits.push_back(new Doge(renderer, filePath.c_str(), 525, 30)); 
        units_large_two_digits.push_back(new Doge(renderer, filePath.c_str(), 541, 30));
        tens_large_digits.push_back(new Doge(renderer, filePath.c_str(), 510, 30));
    }

    // Khởi tạo champion
    champion = new Doge(renderer, "assets/image/champion.png", 390, 150);
    //Khởi tạo hiệu ứng pháo hoa
    for (int i = 1; i <= 8; i++) {
        std::string filePath = "assets/image/fire_works/" + std::to_string(i) + ".png";
        fire_work_1.push_back(new Doge(renderer, filePath.c_str(), 190, 100));
    }
    for (int i = 1; i <= 8; i++) {
        std::string filePath = "assets/image/fire_works/" + std::to_string(i) + ".png";
        fire_work_2.push_back(new Doge(renderer, filePath.c_str(), 690, 100));
    }
}

void Game::setGame() {
    delete doge;
    for (auto& pipe : pipes) {  
        delete pipe;
    }
    sound_playing = false;
    pipes.clear();
    score = 0;
    doge = new Doge(renderer, "assets/image/shiba.png", 429, 285);
    currentBg = background_1;
    currentLand = land_1;
    // Khởi tạo 4 ống với khoảng cách nhau
    for (int i = 0; i < 4; i++) {
        pipes.push_back(new Pipe(renderer, 1080 + i * PIPE_SPACING));
    }
    Mix_PlayMusic(sound->waiting, -1);
}

Game::~Game() {
    delete background_1;
    delete background_2;
    delete background_3;
    delete doge;
    for (auto& pipe : pipes) {
        delete pipe;
    }
    delete land_1;
    delete land_2;
    delete land_3;
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

bool Game::isLose() {
    return lose;
}

void Game::check() {
    // Kiểm tra nếu Doge chạm vào đáy hoặc đỉnh background
    if (doge->getY() <= 0 || doge->getY() + DOGE_HEIGHT >= BACKGROUND_HEIGHT) {
        lose = true;  
    }

    // Kiểm tra va chạm với ống
    for (auto& pipe : pipes) {
        // Nếu Doge vượt qua vị trí ống theo trục X
        if (doge->getX() + DOGE_WIDTH > pipe->getX() && doge->getX() < pipe->getX() + PIPE_WIDTH) {
            // Nếu chạm vào ống trên hoặc dưới
            if (doge->getY() - 5 < pipe->getgapY() || doge->getY() + 5 + DOGE_HEIGHT > pipe->getgapY() + PIPE_GAP) {
                lose = true;                
            }
        }
    }

    for (auto& pipe : pipes) {
        // Nếu doge vừa qua ống
        if (doge->getX() == pipe->getX() + PIPE_WIDTH) {
            score++;
            Mix_PlayChannel(-1, sound->score, 0);
        }
    }

    if (!lose) sound_end = false;
    if (lose && !sound_end) {
        Mix_PlayChannel(-1, sound->end, 0);
        sound_end = true;
    }

    if (score == 99) {
        win = true;
    }
}


void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        if (event.type == SDL_MOUSEBUTTONDOWN ||
            (event.type == SDL_KEYDOWN && (event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_UP))) {
            if (!sound_playing) {
                Mix_PlayMusic(sound->playing, -1);
                sound_playing = true;
            }
            play = true;
            if (!lose) {
                doge->jump();
                Mix_PlayChannel(-1, sound->jump, 0);               
            }
            else {
                play = false;
                setGame();
                lose = false;
            }
        }
    }
}

void Game::render_score() {
    int units_score = score % 10;
    int tens_score = score / 10;
    if (play) {
        if (!lose) {
            if (tens_score == 0) units_large_one_digits[units_score]->render(renderer);
            else {
                units_large_two_digits[units_score]->render(renderer);
                tens_large_digits[tens_score]->render(renderer);
            }
        }
        else {
            int tens_score = score / 10;
            int units_score = score % 10;
            if (tens_score == 0) {
                std::string filePath = "assets/image/small/" + std::to_string(units_score) + ".png";
                units_small_digits = new Doge(renderer, filePath.c_str(), 610, 260);
                units_small_digits->render(renderer);
            }
            else {
                std::string filePath = "assets/image/small/" + std::to_string(units_score) + ".png";
                units_small_digits = new Doge(renderer, filePath.c_str(), 620, 260);
                units_small_digits->render(renderer);
                filePath = "assets/image/small/" + std::to_string(tens_score) + ".png";
                tens_small_digits = new Doge(renderer, filePath.c_str(), 600, 260);
                tens_small_digits->render(renderer);
            }
            
            // Đọc highscore
            std::ifstream file("assets/high_score.txt");
            if (file)  file >> highscore;             
            int tens = highscore / 10;
            int units = highscore % 10;
            if (tens == 0) {
                std::string filePath = "assets/image/small/" + std::to_string(units) + ".png";
                units_high_score = new Doge(renderer, filePath.c_str(), 610, 306);
            }
            else {
                std::string filePath = "assets/image/small/" + std::to_string(units) + ".png";
                units_high_score = new Doge(renderer, filePath.c_str(), 620, 306);
                filePath = "assets/image/small/" + std::to_string(tens) + ".png";
                tens_high_score = new Doge(renderer, filePath.c_str(), 600, 306);
            }
            // In high score
            if (tens_high_score == nullptr) units_high_score->render(renderer);
            else {
                units_high_score->render(renderer);
                tens_high_score->render(renderer);
            }
            // Vẽ medal
            if (score >= 80) medal = new Doge(renderer, "assets/image/gold.png", 440, 267);
            else if(score >= 50) medal = new Doge(renderer, "assets/image/silver.png", 440, 267);
            else if(score >= 20)medal = new Doge(renderer, "assets/image/honor.png", 440, 267);
            medal->render(renderer);
            //Ghi high score mới
            if (score > highscore) {
                std::ofstream outputFile("assets/high_score.txt");
                if (outputFile.is_open()) {
                    outputFile << score;
                    outputFile.close();
                }
            }
        }
    }
}

void Game::render_fireworks() {
    // Vẽ frame hiện tại
    if (currentFrame >= 8) currentFrame = 0;
    fire_work_1[currentFrame]->render(renderer);
    fire_work_2[currentFrame]->render(renderer);
    currentFrame++;
}


void Game::update() {
    if (!lose) {
        for (auto& pipe : pipes) {
            if (score >= 96) pipe->check_win();
            pipe->update();
        }
        currentBg->update();
        currentLand->update();
    }

    if (!win) doge->update();
    if (score == 99 && champion) {
        champion->updateChampion();
    }

}

void Game::render() {
    SDL_RenderClear(renderer);

    if (score == 33) {
        currentBg = background_2;
        currentLand = land_2;
    }
    else if (score == 66) {
        currentBg = background_3;
        currentLand = land_3;
    }
    currentBg->render(renderer);

    // Vẽ Doge
    doge->render(renderer);

    // Vẽ pipes
    for (auto& pipe : pipes) {
        pipe->render(renderer);
    }

    currentLand->render(renderer);

    // Nếu thua game, vẽ Game Over
    if (lose) {
        game_over->render(renderer);
        replay->render(renderer);
    }

    // Vẽ thông báo nếu chưa chơi
    if (!play) message->render(renderer);

    render_score();   
    
    if (score == 99) {
        champion->render(renderer);
        render_fireworks();
        Mix_PlayChannel(-1, sound->fire_work, -1);
        Mix_VolumeChunk(sound->fire_work, 10);
    }

    SDL_RenderPresent(renderer);

}

