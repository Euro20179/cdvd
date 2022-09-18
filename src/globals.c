#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "globals.h"
#include "array.h"

int running = 1;
DvdArray dvds;
IntArray on_click_callbacks;
IntArray on_right_click_callbacks;
int dvd_count = 0;

const char* dvd_file_paths[] = {
    "DVD_Blue.png",
    "DVD_Grey.png",
    "DVD_Orange.png",
    "DVD_Pink.png",
    "DVD_Red.png",
    "DVD_Violet.png",
    "DVD_White.png",
    "DVD_Yellow.png"
};

SDL_Renderer* rend;
SDL_Window* win;


int fps = 60;
SDL_Color backgroundColor = {0, 0, 0, 0};

int WIDTH = 500;
int HEIGHT = 500;
