#include "sound.h"

Sound::Sound() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 128) < 0) {
        std::cout << "SDL_mixer Init Error: " << Mix_GetError() << std::endl;
    }
    
    jump = Mix_LoadWAV("assets/sound/jump.wav");
    if (!jump) {
        std::cout << "L?i t?i âm thanh: " << Mix_GetError() << std::endl;
    }

    score = Mix_LoadWAV("assets/sound/score.wav");
    if (!score) {
        std::cerr << "L?i t?i âm thanh: " << Mix_GetError() << std::endl;
    }

    end = Mix_LoadWAV("assets/sound/end.wav");
    if (!end) {
        std::cerr << "L?i t?i âm thanh: " << Mix_GetError() << std::endl;
    }

}


