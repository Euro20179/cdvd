#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdlib.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <SDL2/SDL.h>

#include "api.h"
#include "dvd.h"
#include "array.h"
#include "globals.h"

int dvd_exit(lua_State* L){
    running = 0;
    return 1;
}

int dvd_add_dvd(lua_State* L){
    int x = lua_tointeger(L, 1);
    int y = lua_tointeger(L, 2);
    const char* file_path = lua_tostring(L, 3);
    if(x == -1) x = rand() % 50;
    if(y == -1) y = rand() % 50;
    Dvd d = request_dvd_init(x, y, 0, 0, file_path ? file_path : "DVD_White.png"); //I would like to put NULL, NULL instead of 0, 0, but that gives warnings
    if(insertDvdArray(&dvds, d) == 1){
	return 2;
    }
    dvd_create_lua_table(&d, L);
    return 1;
}

int dvd_get_dvds(lua_State* L){
    lua_newtable(L);
    for(int i = 0; i < dvd_count; i++){
	Dvd d = dvds.array[i];
	lua_pushinteger(L, i);
	dvd_create_lua_table(&d, L);
	lua_settable(L, -3);
    }
    return 1;
}

int dvd_get_dvd_count(lua_State* L){
    lua_pushinteger(L, dvd_count);
    return 1;
}

int dvd_get_by_id(lua_State* L){
    int id = lua_tointeger(L, 1);
    Dvd* d = get_dvd_by_id(id);
    if(d == NULL)
	return 2;
    dvd_create_lua_table(d, L);
    return 1;
}

int dvd_change_logo_by_id(lua_State* L){
    int id = lua_tointeger(L, 1);
    Dvd* d = get_dvd_by_id(id);
    if(d == NULL)
        return 2;
    const char* file_path = lua_tostring(L, 2);
    if(file_path == NULL){
        dvd_change_logo(d, rend, dvd_file_paths[rand() % sizeof(dvd_file_paths[0])]);
    }
    else{
        dvd_change_logo(d, rend, file_path);
    }
    return 1;
}

int dvd_change_x_by_id(lua_State* L){
    int id = lua_tointeger(L, 1);
    Dvd* d = get_dvd_by_id(id);
    if(d == NULL){
        return 2;
    }
    int x = lua_tointeger(L, 2);
    if(x == NULL)
        x = 0;
    d->x = x;
    return 1;
}

int dvd_change_y_by_id(lua_State* L){
    int id = lua_tointeger(L, 1);
    Dvd* d = get_dvd_by_id(id);
    if(d == NULL){
        return 2;
    }
    int y = lua_tointeger(L, 2);
    if(y == NULL)
        y = 0;
    d->y = y;
    return 1;
}

int dvd_set_pos_by_id(lua_State* L){
    int id = lua_tointeger(L, 1);
    Dvd* d = get_dvd_by_id(id);
    if(d == NULL){
        return 2;
    }
    int y = lua_tointeger(L, 2);
    if(y == NULL)
        y = 0;
    d->y = y;
    int x = lua_tointeger(L, 2);
    if(x == NULL)
        x = 0;
    d->x = x;
    return 1;
}

int sdl_get_window_size(lua_State *L){
    int w, h;
    SDL_GetWindowSize(win, &w, &h);
    lua_newtable(L);
    lua_pushstring(L, "width");
    lua_pushnumber(L, w);
    lua_settable(L, -3);
    lua_pushstring(L, "height");
    lua_pushnumber(L, h);
    lua_settable(L, -3);
    return 1;
}

int sdl_resize(lua_State* L){
    int newW = lua_tointeger(L, 1);
    int newH = lua_tointeger(L, 2);
    int curW, curH;
    SDL_GetWindowSize(win, &curW, &curH);
    if(newW == 0){
        newW = curW;
    }
    if(newH == 0){
        newH = curH;
    }
    SDL_SetWindowSize(win, newW, newH);

    //set the values sdl.width and sdl.height
    lua_getglobal(L, "sdl");
    lua_pushnumber(L, newW);
    lua_setfield(L, -2, "width");
    lua_pushnumber(L, newH);
    lua_setfield(L, -2, "height");

    WIDTH = newW;
    HEIGHT = newH;

    return 1;
}

int sdl_set_fps(lua_State* L){
    int newFps = lua_tointeger(L, 1);
    if(newFps == 0){
	return 2;
    }
    fps = newFps;
    lua_getglobal(L, "sdl");
    lua_pushnumber(L, fps);
    lua_setfield(L, -2, "fps");
    return 1;
}

int sdl_background_color(lua_State* L){
    int r = lua_tointeger(L, 1);
    int g = lua_tointeger(L, 2);
    int b = lua_tointeger(L, 3);
    int a = lua_tointeger(L, 4);
    if(r > 255 || g > 255 || b > 255 || a > 255){
	return 2;
    }
    SDL_SetRenderDrawColor(rend, r, g, b, a);
    return 1;
}

int this_register_on_click(lua_State* L){

    insertIntArray(&on_click_callbacks, luaL_ref(L, LUA_REGISTRYINDEX));
    return 1;
}

int this_register_on_right_click(lua_State* L){

    insertIntArray(&on_right_click_callbacks, luaL_ref(L, LUA_REGISTRYINDEX));
    return 1;
}
