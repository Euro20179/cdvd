#pragma	once

#include "fc.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <lua.h>

#define DVD_WIDTH 98
#define DVD_HEIGHT 43

#define DVD_RENDER_POSITION 1
#define DVD_RENDER_VELOCITY 2
#define DVD_RENDER_BOUNCE_COUNT 4
#define DVD_RENDER_ID 8

extern const char* DVD_IMAGE_LOCATION;

typedef struct {
    double x;
    double y;
    int width;
    int height;
    double xVel;
    double yVel;
    int bounces;
    int canMove;
    int infoToRender;
    const char* file_name;
    SDL_Texture* texture;
    _Bool initialized;
    int id;
} Dvd;

void dvd_render(Dvd* dvd, SDL_Renderer* rend);
void dvd_render_text(Dvd* dvd, SDL_Renderer* rend, FC_Font* font);
Dvd request_dvd_init(int x, int y, int width, int height, const char* file_name);
void dvd_init(Dvd* dvd, SDL_Renderer* rend);
void dvd_goto(Dvd* dvd, int x, int y);
void dvd_move(Dvd* dvd);
int dvd_is_touching_wall(Dvd* dvd, SDL_Window* win);
int dvd_is_in_wall(Dvd* dvd, SDL_Window* win);
void dvd_bounce_x(Dvd* dvd);
void dvd_bounce_y(Dvd* dvd);
void dvd_create_lua_table(Dvd* d, lua_State* L);
void dvd_change_logo(Dvd* dvd, SDL_Renderer* rend, const char* new_logo_file_name);

Dvd* get_dvd_by_id(int id);
