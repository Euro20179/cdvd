#include <stdlib.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <SDL2/SDL.h>

#include "dvd.h"
#include "array.h"
#include "globals.h"

int dvd_exit(lua_State* L){
    running = 0;
    return 1;
}

int dvd_add_dvd(lua_State* L){
    Dvd d = request_dvd_init(rand() % 50, rand() % 50, NULL, NULL, "assets/logos/DVD_White.png");
    if(insertArray(&dvds, d) == 1){
	return 2;
    }
    lua_newtable(L); lua_pushstring(L, "x"); lua_pushnumber(L, d.x);
    lua_settable(L, -3);
    lua_pushstring(L, "y");
    lua_pushnumber(L, d.y);
    lua_settable(L, -3);
    lua_pushstring(L, "width");
    lua_pushnumber(L, d.width);
    lua_settable(L, -3);
    lua_pushstring(L, "height");
    lua_pushnumber(L, d.height);
    lua_settable(L, -3);
    lua_pushstring(L, "xVel");
    lua_pushnumber(L, d.xVel);
    lua_settable(L, -3);
    lua_pushstring(L, "yVel");
    lua_pushnumber(L, d.yVel);
    lua_settable(L, -3);
    lua_pushstring(L, "file_name");
    lua_pushstring(L, d.file_name);
    lua_settable(L, -3);
    return 1;
}
