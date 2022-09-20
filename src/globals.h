#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "array.h"
#include "dvd.h"


extern IntArray on_click_callbacks;
extern IntArray on_right_click_callbacks;
extern IntArray on_bounce_callbacks;
extern IntArray on_middle_click_callbacks;

extern int running;
extern DvdArray dvds;
extern int dvd_count;

extern SDL_Renderer* rend;
extern SDL_Window* win;

extern const char* dvd_file_paths[];

extern int fps;

extern int WIDTH, HEIGHT;
