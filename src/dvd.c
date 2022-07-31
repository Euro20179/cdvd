#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "dvd.h"
#include "globals.h"

const char* DVD_IMAGE_LOCATION = "assets/logos/";

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
    Dvd dvd = { x, y, DVD_WIDTH, DVD_HEIGHT, 1, 0.5, file_name, NULL, 0, rand() };
    dvd_count += 1;
    return dvd;
}

void dvd_init(Dvd* dvd, SDL_Renderer* rend){
    char buf[strlen(DVD_IMAGE_LOCATION) + strlen(dvd->file_name)];
    strcpy(buf, DVD_IMAGE_LOCATION);
    strcat(buf, dvd->file_name);
    dvd->texture = IMG_LoadTexture(rend, buf);
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

void dvd_create_lua_table(Dvd* d, lua_State* L){
    lua_newtable(L);
    lua_pushstring(L, "x");
    lua_pushnumber(L, d->x);
    lua_settable(L, -3);
    lua_pushstring(L, "y");
    lua_pushnumber(L, d->y);
    lua_settable(L, -3);
    lua_pushstring(L, "width");
    lua_pushnumber(L, d->width);
    lua_settable(L, -3);
    lua_pushstring(L, "height");
    lua_pushnumber(L, d->height);
    lua_settable(L, -3);
    lua_pushstring(L, "xVel");
    lua_pushnumber(L, d->xVel);
    lua_settable(L, -3);
    lua_pushstring(L, "yVel");
    lua_pushnumber(L, d->yVel);
    lua_settable(L, -3);
    lua_pushstring(L, "file_name");
    lua_pushstring(L, d->file_name);
    lua_settable(L, -3);
    lua_pushstring(L, "id");
    lua_pushnumber(L, d->id);
    lua_settable(L, -3);
}

void dvd_change_logo(Dvd* dvd, SDL_Renderer* rend, const char* new_logo_file_name){
    dvd->initialized = 0;
    char buf[strlen(DVD_IMAGE_LOCATION) + strlen(new_logo_file_name)];
    strcpy(buf, DVD_IMAGE_LOCATION);
    strcat(buf, new_logo_file_name);
    dvd->file_name = new_logo_file_name;
    dvd->texture = IMG_LoadTexture(rend, buf);
    dvd->initialized = 1;
}

Dvd* get_dvd_by_id(int id){
    for(int i = 0; i < dvd_count; i++){
	if(dvds.array[i].id == id){
	    return &dvds.array[i];
	}
    }
    return NULL;
}
