#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

#include "util.h"

SDL_Rect renderText(SDL_Renderer *rend, TTF_Font *f, const char *text, SDL_Color color, int x, int y){
    SDL_Surface* s;
    SDL_Texture* t;
    s = TTF_RenderText_Solid(f, text, color);
    t = SDL_CreateTextureFromSurface(rend, s);
    SDL_Rect r;
    r.x = x;
    r.y = y;
    TTF_SizeText(f, text, &r.w, &r.h);
    SDL_RenderCopy(rend, t, NULL, &r);
    return r;
}
