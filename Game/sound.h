#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>

class Sound {
public:
	Mix_Chunk* jump;
	Mix_Chunk* score;
	Mix_Chunk* end;
	Sound();
	~Sound();
};



