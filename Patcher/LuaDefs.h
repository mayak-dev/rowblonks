#pragma once

struct lua_State;

typedef int (*lua_CFunction)(lua_State* L);

const auto lua_pushcclosure = reinterpret_cast<void(__cdecl*)(lua_State* L, lua_CFunction fn, int n)>(0x00762A70);
const auto lua_setfield = reinterpret_cast<void(__cdecl*)(lua_State * L, int idx, const char* k)>(0x00762DF0);

#define LUA_REGISTRYINDEX	(-10000)
#define LUA_ENVIRONINDEX	(-10001)
#define LUA_GLOBALSINDEX	(-10002)

#define lua_pushcfunction(L,f) lua_pushcclosure(L, (f), 0)
#define lua_setglobal(L,s)	lua_setfield(L, LUA_GLOBALSINDEX, (s))
