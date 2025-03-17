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

private:

    SDL_Window* window;
    SDL_Renderer* renderer;
    int screenwidth, screenheight;

    bool running, play, lose;

    bool sound_end;
    bool sound_playing;
    int score;

    Background* background_1;
    Background* background_2;
    Background* background_3;

    Background* message;
    Background* game_over;
    Background* replay;

    Doge* doge;

    std::vector<Pipe*> pipes;
    std::vector<Doge*> units_large_one_digits;
    std::vector<Doge*> units_large_two_digits;
    std::vector<Doge*> tens_large_digits;

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
};