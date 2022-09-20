#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "array.h"
#include "dvd.h"
#include "globals.h"
#include "util.h"

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

void dvd_render_text(Dvd* d, SDL_Renderer* rend, TTF_Font* font){
    int yOffset = 0;
    SDL_Color white = {255, 255, 255};
    if(d->infoToRender & DVD_RENDER_POSITION){
        char str[100];
        sprintf(str, "(%.01f , %.01f )", d->x, d->y);

        SDL_Rect r = renderText(rend, font, str, white, d->x + d->width, d->y + yOffset);

        yOffset += r.h;
    }
    if(d->infoToRender & DVD_RENDER_VELOCITY){
        char str[100];
        sprintf(str, "(%f, %f)px/f", d->xVel, d->yVel);
        SDL_Rect r = renderText(rend, font, str, white, d->x + d->width, d->y + yOffset);
        yOffset += r.h;
    }
    if(d->infoToRender & DVD_RENDER_BOUNCE_COUNT){
        char str[100];
        sprintf(str, "b: %d", d->bounces);
        SDL_Rect r = renderText(rend, font, str, white, d->x + d->width, d->y + yOffset);
        yOffset += r.h;
    }
}

Dvd request_dvd_init(int x, int y, int width, int height, const char* file_name){
    Dvd dvd = { x, y, DVD_WIDTH, DVD_HEIGHT, 1, 0.5, 0, 1, DVD_RENDER_POSITION | DVD_RENDER_BOUNCE_COUNT | DVD_RENDER_VELOCITY, file_name, NULL, 0, rand() };
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
    if(dvd->canMove){
        dvd->x += dvd->xVel;
        dvd->y += dvd->yVel;
    }
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

int dvd_is_in_wall(Dvd* dvd, SDL_Window* win){
    int w, h;
    SDL_GetWindowSize(win, &w, &h);
    return
        (dvd->x <= 0 - dvd->width / 10) || (dvd->x + dvd->width >= w + dvd->width / 10) ||
        (dvd->y <= 0 - dvd->height / 10) || (dvd->y + dvd->height >= h + dvd->height / 10);

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
    lua_pushstring(L, "bounces");
    lua_pushinteger(L, d->bounces);
    lua_settable(L, -3);
    lua_pushstring(L, "render_fields");
    lua_pushinteger(L, d->infoToRender);
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
    for(int i = 0; i < dvds.used; i++){
	if(dvds.array[i].id == id){
	    return &dvds.array[i];
	}
    }
    return NULL;
}
