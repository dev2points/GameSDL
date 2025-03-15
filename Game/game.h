#pragma once

#include <SDL.h>
#include <SDL_mixer.h>3
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

private:

    SDL_Window* window;
    SDL_Renderer* renderer;
    int screenwidth, screenheight;
    bool running, play, lose;

    bool sound_end;
    int score;

    Background* background;
    Background* message;
    Background* game_over;
    Background* replay;
    Doge* doge;
    std::vector<Pipe*> pipes;
    Pipe* pipe;
    Land* land;
    Sound* sound;
};