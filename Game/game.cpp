#include "game.h"

#include <iostream>
#include <string>
#include<fstream>
#include <algorithm>


Game::Game(int screenWidth, int screenHeight) {

    screenwidth = screenWidth;
    screenheight = screenHeight;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "SDL Init Failed!\n";
        running = false;
        return;
    }

    window = SDL_CreateWindow("Flappy Adventure", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
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
    if (TTF_Init() == -1) {
        std::cerr << "Error initializing SDL_ttf: " << TTF_GetError() << std::endl;
    }

    // Khởi tạo sound
    sound = new Sound();
    sound_end = false;
    sound_playing = false;

    score = 0;     
        
    play = false; // Kiểm tra trạng thái bắt đầu chơi
    running = true; //  Trạng thái bắt đầu chạy chương trình
    lose = false; //    Thua game
	bool reverse = false;
   
    // Khởi tạo Background, Doge và Land
    // Khởi tạo pointer để tồn tại xuyên suốt đến khi delete
    background_1 = new Background(renderer, "assets/image/background_1.jpg", screenWidth, BACKGROUND_HEIGHT , 0);
    background_2 = new Background(renderer, "assets/image/background_2.jpg", screenWidth, BACKGROUND_HEIGHT , 0);
    background_3 = new Background(renderer, "assets/image/background_3.jpg", screenWidth, BACKGROUND_HEIGHT , 0);
    background_4 = new Background(renderer, "assets/image/background_4.png", screenWidth, BACKGROUND_HEIGHT , 0);
    message = new Background(renderer, "assets/image/message.png", (screenWidth - MESSAGE_WIDTH) / 2, (screenHeight - MESSAGE_HEIGHT) / 2 - 50, 1);

    bird_1 = new Doge(renderer, "assets/image/bird_1.png", 429, 230, 1);
    bird_2 = new Doge(renderer, "assets/image/bird_2.png", 429, 230, 1);
    bird = bird_1;

    effect_1 = new Background(renderer, "assets/image/snow.png", screenWidth, screenheight, 0);
    effect_2 = new Background(renderer, "assets/image/rain_drop.png", screenWidth, screenheight, 0);
    
    game_over = new Background(renderer, "assets/image/gameOver.png", (screenWidth - 250) / 2, (screenHeight - 209) / 2 -50, 1);
    replay = new Background(renderer, "assets/image/replay.png", (screenWidth - 100) / 2, (screenHeight - 56) / 2 + 100 , 1);
    sound_1 = new Doge(renderer, "assets/image/sound_1.png", 5, 0, 5);
    sound_0 = new Doge(renderer, "assets/image/sound_0.png", 5, 0, 5);
    sound_check = sound_1;

    // Khởi tạo 4 ống với khoảng cách nhau
    for (int i = 0; i < 4; i++) {
        pipes.push_back(new Pipe(renderer, 1000 + i * PIPE_SPACING));
    }

    land_1 = new Land(renderer, "assets/image/land_1.jpg", screenWidth, BACKGROUND_HEIGHT);
    land_2 = new Land(renderer, "assets/image/land_2.jpg", screenWidth, BACKGROUND_HEIGHT - 7);
    land_3 = new Land(renderer, "assets/image/land_3.jpg", screenWidth, BACKGROUND_HEIGHT);
    land_4 = new Land(renderer, "assets/image/land_4.png", screenWidth, BACKGROUND_HEIGHT);

    currentBg = background_1;
    currentLand = land_1;
    nextBg = nullptr;
    nextLand = nullptr;

    Mix_PlayMusic(sound->waiting, -1);
    Mix_VolumeMusic(20);

    // Khởi tạo chữ số lớn hiển thị score
    for (int i = 0; i < 10; i++) {
        std::string filePath = "assets/image/large/" + std::to_string(i) + ".png";
        SDL_Surface* surface = IMG_Load(filePath.c_str());
        if (surface) {
            large_digits.push_back(SDL_CreateTextureFromSurface(renderer, surface));
            SDL_FreeSurface(surface);
        }
        else {
            large_digits.push_back(nullptr); // Tránh lỗi nếu ảnh không load được
        }
    }

    // Khởi tạo chữ số bé 
    for (int i = 0; i < 10; i++) {
        std::string filePath = "assets/image/small/" + std::to_string(i) + ".png";
        SDL_Surface* surface = IMG_Load(filePath.c_str());
        if (surface) {
            small_digits.push_back(SDL_CreateTextureFromSurface(renderer, surface));
            SDL_FreeSurface(surface);
        }
        else {
            small_digits.push_back(nullptr); // Tránh lỗi nếu ảnh không load được
        }
    }
    // Đọc high highscore từ file
    std::ifstream file("assets/high_score.txt");
    if (file) file >> highscore;	

    //Khởi tạo hiệu ứng pháo hoa
    for (int i = 1; i <= 8; i++) {
        std::string filePath = "assets/image/fire_works/" + std::to_string(i) + ".png";
        fire_work_1.push_back(new Doge(renderer, filePath.c_str(), 150, 100, 0));
    }
    for (int i = 1; i <= 8; i++) {
        std::string filePath = "assets/image/fire_works/" + std::to_string(i) + ".png";
        fire_work_2.push_back(new Doge(renderer, filePath.c_str(), 650, 100, 0));
    }

    lastSpawnTime = SDL_GetTicks(); // Lấy thời gian khi game khởi động để tạo powerup
}

void Game::setGame(){
   // textTexture;
    for (auto& pipe : pipes) {  
        delete pipe;
    }
    sound_playing = false;
    pipes.clear();
    score = 0;
    bird = bird_1;
    bird->set_x(429);
    bird->set_y(230);
    currentBg = background_1;
    currentLand = land_1;
    Fire_work = false;
	reverse = false;
	is_effect = false;
    Pipe::set_speed();
    // Khởi tạo 4 ống với khoảng cách nhau
    for (int i = 0; i < 4; i++) {
        pipes.push_back(new Pipe(renderer, 1080 + i * PIPE_SPACING));
    }
    Mix_HaltChannel(-1);// Dừng phát âm thanh
    Mix_PlayMusic(sound->waiting, -1);

    powerUps.clear();
    shield = false;
    is_increase_Speed = false; 
    is_decrease_Speed = false;
    jump_high = false;
    jump_low = false;
}

Game::~Game() {
    delete background_1;
    delete background_2;
    delete background_3;
    delete bird;
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
    // Kiểm tra nếu Bird chạm vào đáy hoặc đỉnh background
    if (bird->getY() <= 0 || bird->getY() + BIRD_HEIGHT >= BACKGROUND_HEIGHT) {
        lose = true;  
    }

    checkPowerUpCollision();

    // Kiểm tra va chạm với ống
    if (!shield) {
        for (auto& pipe : pipes) {
           SDL_Rect birdRect = bird->getRect(); // Lấy hitbox của Bird
           SDL_Rect topPipeRect = { pipe->getX(), 0, PIPE_WIDTH, pipe->getgapY() }; // Ống trên
           SDL_Rect bottomPipeRect = { pipe->getX(), pipe->getgapY() + PIPE_GAP, PIPE_WIDTH, screenheight - (pipe->getgapY() + PIPE_GAP) }; // Ống dưới

            // Kiểm tra nếu Bird va chạm với ống trên hoặc ống dưới
           if (SDL_HasIntersection(&birdRect, &topPipeRect) || SDL_HasIntersection(&birdRect, &bottomPipeRect)) {
               lose = true;
           }
       }
    }


    for (auto& pipe : pipes) {
        // Kiểm tra nếu Bird vừa vượt qua cạnh phải của ống
        if (bird->getX() >= pipe->getX() + PIPE_WIDTH && !pipe->isScored) {
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
    if (score > 0 && score % 10 == 0) {
        if (!increase_pipe_speed) {
            if(PIPE_SPEED < 11)   PIPE_SPEED += 0.1;
            if(PIPE_SPACING < 500)    PIPE_SPACING += 5;
            increase_pipe_speed = true;
        }
    }
    else increase_pipe_speed = false;

}


void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            int sound_X = sound_check->getX();
            int sound_Y = sound_check->getY();
            if (x >= sound_X && y >= sound_Y && x <= sound_X + 80 && y <= sound_Y + 49) { // Kiểm tra click chuột có nằm trên sound không
                if (sound_check == sound_1) {
                    sound_check = sound_0;
                    Mix_Volume(-1, 0);  // Tắt tiếng tất cả các sound effect
                    Mix_VolumeMusic(0); // Tắt tiếng nhạc nền
                }
                else {
                    sound_check = sound_1;
                    Mix_Volume(-1, MIX_MAX_VOLUME); // Bật lại sound effect
                    Mix_VolumeMusic(20); // Bật lại nhạc nền

                }
            }
            else {
                if (!sound_playing) {
                    Mix_PlayMusic(sound->playing, -1);
                    sound_playing = true;
                }
                play = true;
                bird->jump();
                Mix_PlayChannel(-1, sound->jump, 0);
            }
            if (lose) {
                int X = replay->get_x();
                int Y = replay->get_y();
                if (x >= X && y >= Y && x <= X + 100 && y <= Y + 56) { // Kiểm tra click chuột có nằm trên replay không
                    play = false;
                    setGame();
                    lose = false;
                }
            }
        }
        if(!lose){
            if (event.type == SDL_KEYDOWN && (event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_UP)) {
                if (!sound_playing) {
                    Mix_PlayMusic(sound->playing, -1);
                    sound_playing = true;
                }
                play = true;
                bird->jump();
                Mix_PlayChannel(-1, sound->jump, 0);
            }
        }
    }
}

void Game::render_score() {
    if (!play) return; // Không hiển thị nếu game chưa bắt đầu

    std::string scoreStr = std::to_string(score);  // Chuyển điểm thành chuỗi
    int numDigits = scoreStr.length();

    int digitWidth = 30;   // Kích thước chữ số 
    int digitHeight = 50;
	int totalWidth = numDigits * digitWidth; // Tổng chiều rộng của điểm
    int startX = (screenwidth - totalWidth) / 2; // Căn giữa màn hình

    if (!lose) { // Khi chưa thua, hiển thị điểm lớn
        for (char c : scoreStr) {
            int digit = c - '0';
            SDL_Rect destRect = { startX, 50, digitWidth, digitHeight }; // Vị trí trên màn hình
            SDL_RenderCopy(renderer, large_digits[digit], nullptr, &destRect);
            startX += digitWidth;
        }
    }
    else { // Khi thua, hiển thị điểm nhỏ + high score
        
        if (medal) medal->render(renderer);
        else {
            // Hiển thị huy chương
            // Ghi high score mới
            if (score > highscore) {
                std::ofstream outputFile("assets/high_score.txt");
                if (outputFile.is_open()) {
                    outputFile << score;
                    outputFile.close();
                }
                medal = new Doge(renderer, "assets/image/champion.png", 400, 267, 0);
				highscore = score;            
                Fire_work = true;
            }
            else if (score >= highscore * 3 / 4)    medal = new Doge(renderer, "assets/image/gold.png", 400, 267, 0);                
            else if (score >= highscore / 2)     medal = new Doge(renderer, "assets/image/silver.png", 400, 267, 0);
            else if (score >= highscore / 4)     medal = new Doge(renderer, "assets/image/honor.png", 400, 267, 0);
        }
        int smallY = 260, highY = 306;
        startX = 585 - (numDigits * 10); // Dịch vị trí để căn chỉnh
        for (char c : scoreStr) {
            int digit = c - '0';
            SDL_Rect destRect = { startX, smallY, 20, 30 };
            SDL_RenderCopy(renderer, small_digits[digit], nullptr, &destRect);           
            startX += 20;
        }

        std::string highScoreStr = std::to_string(highscore);

        startX = 585 - (highScoreStr.length() * 10);
        for (char c : highScoreStr) {
            int digit = c - '0';
            SDL_Rect destRect = { startX, highY, 20, 30 };
            SDL_RenderCopy(renderer, small_digits[digit], nullptr, &destRect);
            startX += 20;
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
    if (score % 30 == 0) {  // Mỗi khi score chia hết cho 30
        int bgIndex = (score / 30) % 4; // Lấy chỉ số background (0, 1, 2, 3 lặp lại)

        if (bgIndex == 0) {
            currentBg = background_1;
            currentLand = land_1;
        }
        else if (bgIndex == 1) {
            currentBg = background_2;
            currentLand = land_2;
        }
        else if (bgIndex == 2) {
            currentBg = background_3;
            currentLand = land_3;
        }
        else if (bgIndex == 3) {
            currentBg = background_4;
            currentLand = land_4;
        }
    }
    if (score % 50 == 0) {
        int Bird_index = score / 50 % 2;
        if (Bird_index == 1) bird = bird_2;
        else bird = bird_1;
        bird->set_y(bird->getY() - 10);
    }
    if (!lose) {
        for (int i = 0; i < 4; i++) {
            pipes[i]->update(pipes[(i + 3) % 4]->getX());
        }
        currentBg->update();
        currentLand->update();
        bird->update_src();

    }
    bird->update();
    if(score >= 50) reverse = (score % 50) < 10;
    
    if (score > 0 && !is_effect) {
        if (score % 25 == 0) {
            is_effect = true;
			int x = rand() % 2;
			if (x == 0) effect = effect_1;
			else effect = effect_2;
        }
    }
    if (score > 0 && score % 35 == 0) is_effect = false;

	if (is_effect) effect->effect_update();
	

    if (play && !lose) {
        // Tạo vật phẩm mỗi 5 giây
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastSpawnTime > 5000) {
            spawnPowerUp();
            lastSpawnTime = currentTime; // Cập nhật thời điểm spawn gần nhất
        }
        updatePowerUps();
        for (Doge* powerup : powerUps) {
            powerup->update_powerup();
        }
        if (is_increase_Speed && SDL_GetTicks() - increase_speed_Time > 3000) { // Sau 3 giây
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
        if (jump_high && SDL_GetTicks() - start_jump_high > 5000) { // Sau 5 giây
            bird->decrease_velocity();           
            jump_high = false;
        }
        if (jump_low && SDL_GetTicks() - start_jump_low > 5000) { // Sau 5 giây
            bird->increase_velocity();         
            jump_low = false;
        }
    }
}

void Game::render() {
    SDL_RenderClear(renderer);
    
    currentBg->render(renderer);    

    // Vẽ pipes
    for (auto& pipe : pipes) {
        pipe->render(renderer);
    }

    // Vẽ Bird
    bird->render_bird(renderer);

    currentLand->render(renderer);    

    // Vẽ thông báo nếu chưa chơi
    if (!play) message->render(renderer);
    

    // Vẽ vật phẩm hỗ trợ
    for (auto& powerUp : powerUps) {
        powerUp->render(renderer);
    }
    if(play && !lose) render_active();

    // Nếu thua game, vẽ Game Over
    if (lose) {
        game_over->render(renderer);
        replay->render(renderer);
    }

	if (is_effect) effect->render(renderer);

    // Vẽ điểm số
    render_score();   
    if (play && !lose) render_highscore();
    
    if (Fire_work) {
        render_fireworks();
        Mix_PlayChannel(-1, sound->fire_work, -1);
        Mix_VolumeChunk(sound->fire_work, 10);
    }
    sound_check->render(renderer);

    SDL_RenderPresent(renderer);

}

void Game::spawnPowerUp() {
    active.push_back( new Doge(renderer, "assets/image/power_up/shield.png", 10, 65, 0));
    active.push_back( new Doge(renderer, "assets/image/power_up/speed_up.png", 10, 118, 0));
    active.push_back( new Doge(renderer, "assets/image/power_up/speed_down.png", 10, 178, 0));
    active.push_back( new Doge(renderer, "assets/image/power_up/jump_high.png", 10, 228, 0));
    active.push_back(new Doge(renderer, "assets/image/power_up/jump_low.png", 10, 273, 0));


    int randX = rand() % (screenwidth - 429 - 50) + 429 + 50;// Xuất hiện vật phẩm cách vị trí doge 50 pixel
    int randType = rand() % 5; // 0: Shield, 1: Speed Up, 2: Speed Down
    std::string filePath;

    switch (randType) {
    case 0: filePath = "assets/image/power_up/shield.png"; break;
    case 1: filePath = "assets/image/power_up/speed_up.png"; break;
    case 2: filePath = "assets/image/power_up/speed_down.png"; break;
    case 3: filePath = "assets/image/power_up/jump_high.png"; break;
    case 4: filePath = "assets/image/power_up/jump_low.png"; break;
    }

    powerUps.push_back(new Doge(renderer, filePath.c_str(), randX, 0, 0)); // Xuất hiện trên đỉnh màn hình
}

void Game::updatePowerUps() {
    for (size_t i = 0; i < powerUps.size(); i++) {
        powerUps[i]->update_powerup(); 

        // Nếu vật phẩm rơi khỏi màn hình, xóa khỏi danh sách
        if (powerUps[i]->getY() > BACKGROUND_HEIGHT) {
            delete powerUps[i];
            powerUps.erase(powerUps.begin() + i);
            i--; // Điều chỉnh lại chỉ mục 
        }
    }
}

void Game::checkPowerUpCollision() {
    SDL_Rect dogeRect = bird->getRect(); // Lấy hitbox của nhân vật chính

    for (size_t i = 0; i < powerUps.size(); i++) {
        SDL_Rect powerUpRect = powerUps[i]->getRect(); // Lấy hitbox của vật phẩm

        // Kiểm tra va chạm
        if (SDL_HasIntersection(&dogeRect, &powerUpRect)) {
            // Xử lý hiệu ứng dựa trên loại vật phẩm
            if (powerUps[i]->getFilePath() == "assets/image/power_up/shield.png") {
                shield = true;
                shieldStartTime = SDL_GetTicks(); // Lưu thời gian nhận shield
            }
            else if (powerUps[i]->getFilePath() == "assets/image/power_up/speed_up.png") {
                increase_speed_Time = SDL_GetTicks(); // Lưu thời điểm thay đổi tốc độ
                if (!is_increase_Speed) {
                    Pipe::increase_Speed();
                    is_increase_Speed = true;
                }
            }
            else if (powerUps[i]->getFilePath() == "assets/image/power_up/speed_down.png") {                
                decrease_speed_Time = SDL_GetTicks();
                if (!is_decrease_Speed) {
                    is_decrease_Speed = true;
                    Pipe::decrease_Speed();
                }
            }
            else if (powerUps[i]->getFilePath() == "assets/image/power_up/jump_high.png") {                
                start_jump_high = SDL_GetTicks();
                if (!jump_high) {
                    bird->increase_velocity();
                    jump_high = true;
                }
            }
            else if (powerUps[i]->getFilePath() == "assets/image/power_up/jump_low.png") {
                start_jump_low = SDL_GetTicks();
                if (!jump_low) {
                    bird->decrease_velocity();
                    jump_low = true;
                }
            }     
            // Xóa vật phẩm sau khi ăn
            delete powerUps[i];
            powerUps.erase(powerUps.begin() + i);
            i--; // Điều chỉnh lại chỉ mục sau khi xóa phần tử
        }
    }
}

void Game::render_active() {
    Uint32 current = SDL_GetTicks();
    if (shield) {
        active[0]->render(renderer);
        SDL_Rect timeBar0 = { 60, 90, (float)(3000 - (current - shieldStartTime)) / 3000 * 180, 10 };
        SDL_SetRenderDrawColor(renderer, 200, 10, 10, 255);
        SDL_RenderFillRect(renderer, &timeBar0);
    }
    if (is_increase_Speed) {
        active[1]->render(renderer);
        SDL_Rect timeBar1 = { 60, 150, (float)(3000 - (current - increase_speed_Time)) / 3000 * 180, 10 };
        SDL_SetRenderDrawColor(renderer, 200, 10, 10, 255);
        SDL_RenderFillRect(renderer, &timeBar1);
    }
    if (is_decrease_Speed) {
        active[2]->render(renderer);
        SDL_Rect timeBar2 = { 60, 192, (float)(3000 - (current - decrease_speed_Time)) / 3000 * 180, 10 };
        SDL_SetRenderDrawColor(renderer, 200, 10, 10, 255);
        SDL_RenderFillRect(renderer, &timeBar2);
    }
    if (jump_high) {
        active[3]->render(renderer);
        SDL_Rect timeBar3 = { 60, 250, (float)(5000 - (current - start_jump_high)) / 5000 * 180, 10 };
        SDL_SetRenderDrawColor(renderer, 200, 10, 10, 255);
        SDL_RenderFillRect(renderer, &timeBar3);
    }
    if (jump_low) {
        active[4]->render(renderer);
        SDL_Rect timeBar4 = { 60, 290, (float)(5000 - (current - start_jump_low)) / 5000 * 180, 10 };
        SDL_SetRenderDrawColor(renderer, 200, 10, 10, 255);
        SDL_RenderFillRect(renderer, &timeBar4);
    }
}

void Game::render_highscore() {
    // Xác định giá trị hiển thị
    int newDisplayHighscore = std::max(score, highscore);

    // Chỉ cập nhật nếu giá trị hiển thị thay đổi
    if (newDisplayHighscore != displayHighscore) {
        displayHighscore = newDisplayHighscore;

        // Giải phóng texture cũ nếu có
        if (textTexture != nullptr) {
            SDL_DestroyTexture(textTexture);
            textTexture = nullptr;
        }

        // Load font
        TTF_Font* font = TTF_OpenFont("assets/font.ttf", 30);
        if (!font) {
            std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
            return;
        }

        // Tạo text mới
        SDL_Color textColor = { 240, 230, 250, 255 }; // Màu trắng
        std::string highScoreText = "High Score: " + std::to_string(displayHighscore);
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, highScoreText.c_str(), textColor);
        if (!textSurface) {
            std::cerr << "Failed to create text surface: " << TTF_GetError() << std::endl;
            TTF_CloseFont(font);
            return;
        }

        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);
        TTF_CloseFont(font);

        // Cập nhật vị trí hiển thị
        int textW = 0, textH = 0;
        SDL_QueryTexture(textTexture, NULL, NULL, &textW, &textH);
        textRect = { screenwidth - textW - 10, 10, textW, textH };
    }

    // Vẽ highscore lên màn hình
    if (textTexture != nullptr) {
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    }
}


