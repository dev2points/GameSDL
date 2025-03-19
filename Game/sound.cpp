#include "sound.h"

Sound::Sound() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 128) < 0) {
        std::cout << "SDL_mixer Init Error: " << Mix_GetError() << std::endl;
    }
    
    jump = Mix_LoadWAV("assets/sound/jump.wav");
    if (!jump) {
        std::cout << "Lỗi tải âm thanh: " << Mix_GetError() << std::endl;
    }

    score = Mix_LoadWAV("assets/sound/score.wav");
    if (!score) {
        std::cerr << "Lỗi tải âm thanh: " << Mix_GetError() << std::endl;
    }

    end = Mix_LoadWAV("assets/sound/end.wav");
    if (!end) {
        std::cerr << "Lỗi tải âm thanh: " << Mix_GetError() << std::endl;
    }

    fire_work = Mix_LoadWAV("assets/sound/fire_work.wav");
    if (!fire_work) {
        std::cerr << "Lỗi tải âm thanh: " << Mix_GetError() << std::endl;
    }

    waiting = Mix_LoadMUS("assets/sound/waiting_music.mp3");
    if (!waiting) {
        std::cerr << "Lỗi tải âm thanh: " << Mix_GetError() << std::endl;
    }

    playing = Mix_LoadMUS("assets/sound/playing_music.mp3");
    if (!playing) {
        std::cerr << "Lỗi tải âm thanh: " << Mix_GetError() << std::endl;
    }
}


