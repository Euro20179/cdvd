#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "dvd.h"
#include "globals.h"

const char* DVD_IMAGE_LOCATION = "assets/logos";

void dvd_render(Dvd *dvd, SDL_Renderer *rend){
    if(dvd->initialized == 0){
	dvd_init(dvd, rend);
    }
    SDL_Rect dest;
    dest.x = dvd->x;
    dest.y = dvd->y;
    dest.w = dvd->width;
    dest.h = dvd->height;
    SDL_RenderCopy(rend, dvd->texture, NULL, &dest);
}

Dvd request_dvd_init(int x, int y, int width, int height, const char* file_name){
    Dvd dvd = { x, y, DVD_WIDTH, DVD_HEIGHT, 1, 0.5, file_name, NULL, 0, rand() * rand() };
    dvd_count += 1;
    return dvd;
}

void dvd_init(Dvd* dvd, SDL_Renderer* rend){
    dvd->texture = IMG_LoadTexture(rend, dvd->file_name);
    dvd->initialized = 1;
}

void dvd_goto(Dvd* dvd, int x, int y){
    dvd->x = x;
    dvd->y = y;
}

void dvd_move(Dvd* dvd){
    dvd->x += dvd->xVel;
    dvd->y += dvd->yVel;
}

//0 == no touching
//1 == x
//2 == y
int dvd_is_touching_wall(Dvd* dvd, SDL_Window* win){
    int w, h;
    SDL_GetWindowSize(win, &w, &h);
    if(dvd->x <= 0) return 1;
    if(dvd->x + dvd->width >= w) return 1;
    if(dvd->y <= 0) return 2;
    if(dvd->y + dvd->height >= h) return 2;
    return 0;
}

void dvd_bounce_x(Dvd* dvd){
    dvd->xVel *= -1;
}
void dvd_bounce_y(Dvd* dvd){
    dvd->yVel *= -1;
}
