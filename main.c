#include <SDL2/SDL.h>

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_ttf.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>

#include "src/array.h"
#include "src/dvd.h"
#include "src/api.h"
#include "src/globals.h"
#include "src/config.h"

//int multiplication(lua_State* L);
//
//
//TODO: add explosion effect when dvds collide

void create_dvd_module_object(lua_State* L){
    lua_newtable(L);
    lua_pushcfunction(L, dvd_exit);
    lua_setfield(L, -2, "exit");
    lua_pushcfunction(L, dvd_pop);
    lua_setfield(L, -2, "pop");
    lua_pushcfunction(L, dvd_add_dvd );
    lua_setfield(L, -2, "add");
    lua_pushcfunction(L, dvd_get_dvds );
    lua_setfield(L, -2, "get_all");
    lua_pushcfunction(L, dvd_get_dvd_count );
    lua_setfield(L, -2, "count");
    lua_pushcfunction(L, dvd_get_by_id);
    lua_setfield(L, -2, "get_by_id");
    lua_pushcfunction(L, dvd_change_logo_by_id);
    lua_setfield(L, -2, "change_logo_by_id");
    lua_pushcfunction(L, dvd_change_x_by_id);
    lua_setfield(L, -2, "change_x_by_id");
    lua_pushcfunction(L, dvd_change_y_by_id);
    lua_setfield(L, -2, "change_y_by_id");
    lua_pushcfunction(L, dvd_bounce_by_id);
    lua_setfield(L, -2, "bounce_by_id");
    lua_pushcfunction(L, dvd_set_pos_by_id);
    lua_setfield(L, -2, "set_pos_by_id");
    lua_pushcfunction(L, dvd_set_moving_by_id);
    lua_setfield(L, -2, "set_moving_by_id");
    lua_pushcfunction(L, dvd_get_all_dvds);
    lua_setfield(L, -2, "get_all_dvds");
    lua_setglobal(L, "dvd");
}

void create_sdl_module_object(lua_State* L){
    lua_newtable(L);
    lua_pushcfunction(L, sdl_get_window_size);
    lua_setfield(L, -2, "size");
    lua_pushcfunction(L, sdl_resize);
    lua_setfield(L, -2, "resize");
    lua_pushnumber(L, 500);
    lua_setfield(L, -2, "width");
    lua_pushnumber(L, 500);
    lua_setfield(L, -2, "height");
    lua_pushnumber(L, 60);
    lua_setfield(L, -2, "fps");
    lua_pushcfunction(L, sdl_set_fps);
    lua_setfield(L, -2, "set_fps");
    lua_pushcfunction(L, sdl_background_color);
    lua_setfield(L, -2, "background");
    lua_setglobal(L, "sdl");
}

void create_this_module_object(lua_State* L){
    lua_newtable(L);
    lua_pushcfunction(L, this_register_on_click);
    lua_setfield(L, -2, "register_on_click");
    lua_pushcfunction(L, this_register_on_right_click);
    lua_setfield(L, -2, "register_on_right_click");
    lua_pushcfunction(L, this_register_on_middle_click);
    lua_setfield(L, -2, "register_on_middle_click");
    lua_pushcfunction(L, this_register_on_bounce);
    lua_setfield(L, -2, "register_on_bounce");
    lua_setglobal(L, "this");
}

int main(int argc, char ** argv) {
    srand(time(NULL));


    initDvdArray(&dvds, 0);
    initIntArray(&on_click_callbacks, 0);
    initIntArray(&on_right_click_callbacks, 0);
    initIntArray(&on_bounce_callbacks, 0);
    initIntArray(&on_middle_click_callbacks, 0);

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    create_dvd_module_object(L);
    create_sdl_module_object(L);
    create_this_module_object(L);

    // Work with lua API

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("Error initializing SDL: %s\n", SDL_GetError());
    }


    win = SDL_CreateWindow("DVDScreen", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    TTF_Init();
    TTF_Font* font = TTF_OpenFont("/usr/share/fonts/ubuntu/Ubuntu-M.ttf", 10);
    if(!font){
        printf("%s\n", "Font not found");
        return 1;
    }

    SDL_Color White = {255, 255, 255};
    SDL_Surface* infoSurface;
    SDL_Texture* info;

    SDL_Rect message_rect;
    message_rect.x = 0;
    message_rect.y = 0;
    message_rect.w = 10 * 5;
    message_rect.h = 10 * 5;


    char systemPath[30];
    globalConfigPath(systemPath);
    DIR* d;
    struct dirent* dir;
    d = opendir(systemPath);
    if(d){
        while((dir = readdir(d)) != NULL){
            if(luaL_dofile(L,strcat(systemPath,dir->d_name)) == LUA_OK){
                lua_pop(L, lua_gettop(L));
            }
            else{
                fprintf(stderr, "%s\n", lua_tostring(L, -1));
            }
            globalConfigPath(systemPath);
        }
        closedir(d);
    }

    char configPath[150];
    getConfigPath(configPath);

    if( luaL_dofile(L,strcat(configPath, "config.lua")) == LUA_OK){
        lua_pop(L, lua_gettop(L));
    }
    else{
        fprintf(stderr, "%s\n", lua_tostring(L, -1));
        return 1;
    }

    lua_getglobal(L, "sdl");
    lua_getfield(L, -1, "fps");
    fps = lua_tointeger(L, -1);
    if(fps == 0) fps = 60;

    while(running == 1){
        SDL_Event event;

        //hopefuly no one has a good enough computer that can render a googol dvds :weary:
        char str[100];
        sprintf(str, "%d", dvds.used);

        infoSurface = TTF_RenderText_Solid(font, str, White);
        info = SDL_CreateTextureFromSurface(rend, infoSurface);

        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:{
                    lua_getglobal(L, "on_exit");
                    lua_pcall(L, 0, 0, 0);
                    running = 0;
                    break;
                }

                case SDL_MOUSEBUTTONDOWN:{
                    int x, y;
                    Uint32 buttons = SDL_GetMouseState(&x, &y);
                    switch(buttons){
                        case SDL_BUTTON_LMASK:{
                            for(int i = 0; i < on_click_callbacks.used; i++){
                                lua_rawgeti(L, LUA_REGISTRYINDEX, on_click_callbacks.array[i]);
                                lua_pushinteger(L, x);
                                lua_pushinteger(L, y);
                                lua_pcall(L, 2, 1, 0);
                            }
                            break;
                        }
                        case SDL_BUTTON_RMASK: {
                            for(int i = 0; i < on_right_click_callbacks.used; i++){
                                lua_rawgeti(L, LUA_REGISTRYINDEX, on_right_click_callbacks.array[i]);
                                lua_pushinteger(L, x);
                                lua_pushinteger(L, y);
                                lua_pcall(L, 2, 1, 0);
                            }
                            break;
                        }
                        case SDL_BUTTON_MIDDLE: {
                            for(int i = 0; i < on_right_click_callbacks.used; i++){
                                lua_rawgeti(L, LUA_REGISTRYINDEX, on_middle_click_callbacks.array[i]);
                                lua_pushinteger(L, x);
                                lua_pushinteger(L, y);
                                lua_pcall(L, 2, 1, 0);
                            }
                            break;
                        }
                    }
                    break;
                }
            }
        }

        SDL_RenderClear(rend);



        for(int i = 0; i < dvds.used; i++){
            if(&dvds.array[i] == NULL) continue;

            Dvd* d = &dvds.array[i];
            dvd_render(d, rend);
            dvd_move(d);

            switch(dvd_is_touching_wall(d, win)){
                case 1:
                    d->bounces++;

                    for(int i = 0; i < on_bounce_callbacks.used; i++){
                        lua_rawgeti(L, LUA_REGISTRYINDEX, on_bounce_callbacks.array[i]);
                        lua_pushstring(L, "x");
                        lua_pushinteger(L, d->id);
                        dvd_create_lua_table(d, L);
                        lua_pcall(L, 3, 0, 0);
                    }

                    break;

                case 2:
                    d->bounces++;

                    for(int i = 0; i < on_bounce_callbacks.used; i++){
                        lua_rawgeti(L, LUA_REGISTRYINDEX, on_bounce_callbacks.array[i]);
                        lua_pushstring(L, "y");
                        lua_pushinteger(L, d->id);
                        dvd_create_lua_table(d, L);
                        lua_pcall(L, 3, 0, 0);
                    }

                    break;
            }
            if(dvd_is_in_wall(d, win)){
                d->x = 1;
                d->y = 1;
            }
        }

        SDL_RenderCopy(rend, info, NULL, &message_rect);
        SDL_RenderPresent(rend);
        SDL_Delay(1000 / fps);
    }

    SDL_DestroyTexture(info);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);

    SDL_Quit();

    lua_close(L);


    return 0;
}

