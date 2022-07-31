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
    Dvd d = request_dvd_init(rand() % 50, rand() % 50, NULL, NULL, "DVD_White.png");
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
