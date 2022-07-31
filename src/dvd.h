#pragma	once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define DVD_WIDTH 98
#define DVD_HEIGHT 43

extern const char* DVD_IMAGE_LOCATION;

typedef struct {
    double x;
    double y;
    int width;
    int height;
    double xVel;
    double yVel;
    const char* file_name;
    SDL_Texture* texture;
    _Bool initialized;
    long double id;
} Dvd;
/*
    SDL_Surface* surface;
    surface = IMG_Load("assets/logos/DVD_White.png");
    SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surface);
    SDL_FreeSurface(surface);
    SDL_Rect dest;
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    dest.w = DVD_WIDTH;
    dest.h = DVD_HEIGHT;
    dest.x = 0;
    dest.y = 0;
    */

void dvd_render(Dvd* dvd, SDL_Renderer* rend);
Dvd request_dvd_init(int x, int y, int width, int height, const char* file_name);
void dvd_init(Dvd* dvd, SDL_Renderer* rend);
void dvd_goto(Dvd* dvd, int x, int y);
void dvd_move(Dvd* dvd);
int dvd_is_touching_wall(Dvd* dvd, SDL_Window* win);
void dvd_bounce_x(Dvd* dvd);
void dvd_bounce_y(Dvd* dvd);
