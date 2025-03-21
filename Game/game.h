#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>
#include "background.h"
#include "doge.h"
#include "pipe.h"
#include "land.h"
#include "sound.h"



class Game {
public:
    Game(int screenWidth, int screenHeight);
    ~Game();

    bool isRunning() const;
    bool isPlay();
    bool isLose();

    void handleEvents();
    void update();
    void render();
    void check();
    void setGame();

    void render_score();
    void render_fireworks();

    void spawnPowerUp();
    void updatePowerUps();
    void renderPowerUps();
    void checkPowerUpCollision();


private:

    SDL_Window* window;
    SDL_Renderer* renderer;
    int screenwidth, screenheight;

    bool running, play, lose, win;

    bool sound_end;
    bool sound_playing;
    int score;
    bool shield;

    Uint32 lastSpawnTime; // Lưu thời điểm spawn vật phẩm gần nhất
    Uint32 increase_speed_Time = 0; // Thời điểm tăng tốc độ
    Uint32 decrease_speed_Time = 0; // Thời điểm thay đổi tốc độ
    bool is_increase_Speed = false; // Đánh dấu đã tăng tốc độ
    bool is_decrease_Speed = false; // Đánh dấu đã giảm tốc độ
    Uint32 shieldStartTime; // Thời điểm nhận shield



    Background* background_1;
    Background* background_2;
    Background* background_3;

    Background* message;
    Background* game_over;
    Background* replay;

    Doge* doge;

    std::vector <Pipe*> pipes;
    std::vector <Doge*> units_large_one_digits;
    std::vector <Doge*> units_large_two_digits;
    std::vector <Doge*> tens_large_digits;

    Doge* units_small_digits;
    Doge* tens_small_digits;
    Doge* units_high_score;
    Doge* tens_high_score;
    Doge* medal;

    Land* land_1;
    Land* land_2;
    Land* land_3;

    Sound* sound;

    Background* currentBg;
    Background* nextBg;

    Land* currentLand;
    Land* nextLand;

    int highscore;

    std::vector <Doge*> fire_work_1;
    std::vector <Doge*> fire_work_2;
    Doge* champion;
    int currentFrame = 0;

    std::vector<Doge*> powerUps; // Danh sách các vật phẩm hỗ trợ
    int powerUpTimer; // Biến đếm thời gian xuất hiện vật phẩm

};