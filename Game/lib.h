#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class position
{
public:
    short int x, y, angle, state;
    void getPos(const short int x, const short int y);
};
