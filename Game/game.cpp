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

    lastSpawnTime = SDL_GetTicks(); // Lấy thời gian khi game khởi động

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

    checkPowerUpCollision();

    // Kiểm tra va chạm với ống
    if (!shield) {
        for (auto& pipe : pipes) {
            SDL_Rect dogeRect = doge->getRect(); // Lấy hitbox của Doge
            SDL_Rect topPipeRect = { pipe->getX(), 0, PIPE_WIDTH, pipe->getgapY() }; // Ống trên
            SDL_Rect bottomPipeRect = { pipe->getX(), pipe->getgapY() + PIPE_GAP, PIPE_WIDTH, screenheight - (pipe->getgapY() + PIPE_GAP) }; // Ống dưới

            // Kiểm tra nếu Doge va chạm với ống trên hoặc ống dưới
            if (SDL_HasIntersection(&dogeRect, &topPipeRect) || SDL_HasIntersection(&dogeRect, &bottomPipeRect)) {
                lose = true;
            }
        }
    }


    for (auto& pipe : pipes) {
        // Kiểm tra nếu Doge vừa vượt qua cạnh phải của ống
        if (doge->getX() >= pipe->getX() + PIPE_WIDTH && !pipe->isScored) {
            score++;
            Mix_PlayChannel(-1, sound->score, 0);
            pipe->isScored = true; // Đánh dấu ống đã được tính điểm
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
    // Tạo vật phẩm mỗi 5 giây
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastSpawnTime > 5000) { 
        spawnPowerUp();
        lastSpawnTime = currentTime; // Cập nhật thời điểm spawn gần nhất
    }
    for (Doge* powerup : powerUps) {
        powerup->update_powerup();
    }
    if (is_increase_Speed && SDL_GetTicks() - increase_speed_Time > 2000) { // Sau 2 giây
        Pipe::decrease_Speed(); // Reset về tốc độ mặc định 
        is_increase_Speed = false; // Đánh dấu đã reset
    }
    if (is_decrease_Speed && SDL_GetTicks() - decrease_speed_Time > 3000) { // Sau 3 giây
        Pipe::increase_Speed(); // Reset về tốc độ mặc định 
        is_decrease_Speed = false; // Đánh dấu đã reset
    }
    if (shield && SDL_GetTicks() - shieldStartTime > 3000) { // 3 giây
        shield = false;        
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

    // Vẽ vật phẩm hỗ trợ
    for (auto& powerUp : powerUps) {
        powerUp->render(renderer);
    }

    // Vẽ điểm số
    render_score();   
    
    if (score == 99) {
        champion->render(renderer);
        render_fireworks();
        Mix_PlayChannel(-1, sound->fire_work, -1);
        Mix_VolumeChunk(sound->fire_work, 10);
    }

    SDL_RenderPresent(renderer);

}

void Game::spawnPowerUp() {
    int randX = rand() % (screenwidth - 429 - 50) + 429 + 50;// Xuất hiện vật phẩm cách vị trí doge 50 pixel
    int randType = rand() % 3; // 0: Shield, 1: Speed Up, 2: Speed Down
    std::string filePath;

    switch (randType) {
    case 0: filePath = "assets/image/power_up/shield.png"; break;
    case 1: filePath = "assets/image/power_up/speed_up.png"; break;
    case 2: filePath = "assets/image/power_up/speed_down.png"; break;
    }

    powerUps.push_back(new Doge(renderer, filePath.c_str(), randX, 0)); // Xuất hiện trên đỉnh màn hình
}

void Game::updatePowerUps() {
    for (size_t i = 0; i < powerUps.size(); i++) {
        powerUps[i]->update_powerup(); // Vật phẩm rơi xuống với tốc độ 1 pixel/frame

        // Nếu vật phẩm rơi khỏi màn hình, xóa khỏi danh sách
        if (powerUps[i]->getY() > BACKGROUND_HEIGHT) {
            delete powerUps[i];
            powerUps.erase(powerUps.begin() + i);
            i--; // Điều chỉnh lại chỉ mục 
        }
    }
}

void Game::checkPowerUpCollision() {
    SDL_Rect dogeRect = doge->getRect(); // Lấy hitbox của nhân vật chính

    for (size_t i = 0; i < powerUps.size(); i++) {
        SDL_Rect powerUpRect = powerUps[i]->getRect(); // Lấy hitbox của vật phẩm

        // Kiểm tra va chạm
        if (SDL_HasIntersection(&dogeRect, &powerUpRect)) {
            // Xử lý hiệu ứng dựa trên loại vật phẩm
            if (powerUps[i]->getFilePath() == "assets/image/power_up/shield.png") {
                shield = true;
                shieldStartTime = SDL_GetTicks(); // Lưu thời gian nhận shield
                std::cout << "kích hoạt shield" << std::endl;
            }
            else if (powerUps[i]->getFilePath() == "assets/image/power_up/speed_up.png") {
                Pipe::increase_Speed();
                increase_speed_Time = SDL_GetTicks(); // Lưu thời điểm thay đổi tốc độ
                is_increase_Speed = true;
            }
            else if (powerUps[i]->getFilePath() == "assets/image/power_up/speed_down.png") {
                Pipe::decrease_Speed();
                decrease_speed_Time = SDL_GetTicks();
                is_decrease_Speed = true;
            }
            else std :: cout << powerUps[i]->getFilePath();


            // Xóa vật phẩm sau khi ăn
            delete powerUps[i];
            powerUps.erase(powerUps.begin() + i);
            i--; // Điều chỉnh lại chỉ mục sau khi xóa phần tử
        }
    }
}

