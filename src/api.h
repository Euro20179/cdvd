#pragma once

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

int dvd_exit(lua_State* L);
int dvd_get_dvd_count(lua_State* L);
int dvd_get_dvds(lua_State* L);
int dvd_add_dvd(lua_State* L);
int dvd_get_by_id(lua_State* L);
int dvd_change_logo_by_id(lua_State* L);

int sdl_get_window_size(lua_State* L);
int sdl_resize(lua_State* L);
int sdl_set_fps(lua_State* L);
int sdl_background_color(lua_State* L);
