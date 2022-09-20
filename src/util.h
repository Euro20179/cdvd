#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

SDL_Rect renderText(SDL_Renderer* rend, TTF_Font* f, const char* text, SDL_Color color, int x, int y);
