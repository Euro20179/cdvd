#include <SDL2/SDL.h>

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <time.h>

#include "src/array.h"
#include "src/dvd.h"
#include "src/api.h"
#include "src/globals.h"

//int multiplication(lua_State* L);

void create_dvd_module_object(lua_State* L){
    lua_newtable(L);
    lua_pushcfunction(L, dvd_exit);
    lua_setfield(L, -2, "exit");
    lua_pushcfunction(L, dvd_add_dvd );
    lua_setfield(L, -2, "add");
    lua_pushcfunction(L, dvd_get_dvds );
    lua_setfield(L, -2, "get_all");
    lua_pushcfunction(L, dvd_get_dvd_count );
    lua_setfield(L, -2, "count");
    lua_pushcfunction(L, dvd_get_by_id);
    lua_setfield(L, -2, "get_by_id");
    lua_pushcfunction(L, dvd_change_logo_by_id);
    lua_setfield(L, -2, "set_logo_by_id");
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
    lua_setglobal(L, "sdl");
}

int main(int argc, char ** argv) {
    srand(time(NULL));

    initDvdArray(&dvds, 1);

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    create_dvd_module_object(L);
    create_sdl_module_object(L);

    // Work with lua API

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
	printf("Error initializing SDL: %s\n", SDL_GetError());
    }

    win = SDL_CreateWindow("DVDScreen", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 500, SDL_WINDOW_SHOWN);

    rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    if(luaL_dofile(L, "config.lua") == LUA_OK){
	lua_pop(L, lua_gettop(L));
    }

    while(running == 1){
	SDL_Event event;
	while(SDL_PollEvent(&event)){
	    switch(event.type){
		case SDL_QUIT:
		    {
			lua_getglobal(L, "on_exit");
			lua_pcall(L, 0, 0, 0);
			running = 0;
			break;
		    }
	    }
	}
	SDL_RenderClear(rend);
	for(int i = 0; i < dvd_count; i++){
	    if(&dvds.array[i] == NULL) continue;
	    Dvd* d = &dvds.array[i];
	    dvd_render(d, rend);
	    dvd_move(d);
	    switch(dvd_is_touching_wall(d, win)){
		case 1:
		    dvd_bounce_x(d);
		    dvd_change_logo(d, rend, dvd_file_paths[rand() % sizeof(dvd_file_paths[0])]);

		    lua_getglobal(L, "dvd_bounce_x");
		    lua_pushinteger(L, d->id);
		    dvd_create_lua_table(d, L);
		    lua_pcall(L, 2, 0, 0);
		    break;
		case 2:
		    dvd_bounce_y(d);
		    dvd_change_logo(d, rend, "DVD_Blue.png");

		    lua_getglobal(L, "dvd_bounce_y");
		    lua_pushinteger(L, d->id);
		    dvd_create_lua_table(d, L);
		    lua_pcall(L, 2, 0, 0);
		    break;
	    }
	}
	SDL_RenderPresent(rend);
	SDL_Delay(1000 / 60);
    }

    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    lua_close(L);


    return 0;
}

