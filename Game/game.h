#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
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
    void render_active();

    void render_highscore();

private:

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* textTexture;
    SDL_Rect textRect;

    int screenwidth, screenheight;

    bool running, play, lose;

    bool sound_end;
    bool sound_playing;
    bool Fire_work;

    Uint32 lastSpawnTime; // Lưu thời điểm spawn vật phẩm gần nhất
    Uint32 increase_speed_Time = 0; // Thời điểm tăng tốc độ
    Uint32 decrease_speed_Time = 0; // Thời điểm thay đổi tốc độ
    Uint32 shieldStartTime; // Thời điểm nhận shield
    Uint32 start_jump_high; // Thời điểm bắt đầu nhảy cao
    Uint32 start_jump_low; // Thời điểm bắt đầu nhảy thấp
    Uint32 speed_up_Time = 0; // Thời gian tăng tốc độ
    Uint32 speed_down_Time = 0; // Thời gian giảm tốc độ
    Uint32 shield_Time; // Thời gian hiệu lực shield
    Uint32 jump_high_Time; // Thời gian có thể nhảy cao
    Uint32 jump_low_Time; // Thời gian có thể nhảy thấp 


    bool shield;
    bool is_increase_Speed = false; 
    bool is_decrease_Speed = false; 
    bool jump_high = false;
    bool jump_low = false;

    bool increase_pipe_speed = false;



    Background* background_1;
    Background* background_2;
    Background* background_3;
    Background* background_4;

    Background* message;
    Background* game_over;
    Background* replay;

    Doge* setting;

    Doge* bird_1;
    Doge* bird_2;
    Doge* bird;

    Doge* sound_check;
    Doge* sound_1;
    Doge* sound_0;    

    std::vector <Pipe*> pipes;    

    std::vector<SDL_Texture*> large_digits; // Vector lưu texture số lớn
    std::vector<SDL_Texture*> small_digits; // Vector lưu texture số lớn
    Doge* medal;

    Land* land_1;
    Land* land_2;
    Land* land_3;
    Land* land_4;

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
    std::vector<Doge*> active;// danh sách vật phẩm có hiệu lực
    int powerUpTimer; // Biến đếm thời gian xuất hiện vật phẩm

    int displayHighscore;

};

