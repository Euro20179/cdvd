#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "array.h"
#include "dvd.h"

extern int running;
extern DvdArray dvds;
extern int dvd_count;

extern SDL_Renderer* rend;
extern SDL_Window* win;

extern const char* dvd_file_paths[];

extern int fps;
