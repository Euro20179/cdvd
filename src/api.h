#pragma once

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

int dvd_exit(lua_State* L);
int dvd_get_dvds(lua_State* L);
int dvd_add_dvd(lua_State* L);
