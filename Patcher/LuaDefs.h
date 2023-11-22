#pragma once

#include "LuaHelpers.h"

struct lua_State;

typedef int (*lua_CFunction)(lua_State* L);

const auto lua_pushcclosure = reinterpret_cast<void(__cdecl*)(lua_State* L, lua_CFunction fn, int n)>(0x00762A70);

const auto lua_setfield = reinterpret_cast<void(__cdecl*)(lua_State* L, int idx, const char* k)>(0x00762DF0);
const auto lua_getfield = reinterpret_cast<void(__cdecl*)(lua_State* L, int idx, const char* k)>(0x00762BB0);

const auto luaL_checklstring = reinterpret_cast<const char* (__cdecl*)(lua_State* L, int narg, size_t* len)>(0x00764190);

const auto luaL_checkudata = reinterpret_cast<void* (__cdecl*)(lua_State* L, int ud, const char* tname)>(0x00764080);

const auto luaL_error = reinterpret_cast<int(__cdecl*)(lua_State* L, const char* fmt, ...)>(0x00763710);

const auto lua_settop = reinterpret_cast<void(__cdecl*)(lua_State* L, int idx)>(0x00762370);

#define LUA_REGISTRYINDEX	(-10000)
#define LUA_ENVIRONINDEX	(-10001)
#define LUA_GLOBALSINDEX	(-10002)

#define lua_pushcfunction(L,f)  lua_pushcclosure(L, (f), 0)

#define lua_setglobal(L,s)      lua_setfield(L, LUA_GLOBALSINDEX, (s))
#define lua_getglobal(L,s)      lua_getfield(L, LUA_GLOBALSINDEX, (s))

#define luaL_checkstring(L,n)   (luaL_checklstring(L, (n), NULL))

#define lua_pop(L,n)            lua_settop(L, -(n)-1)

namespace RBX
{
	class ScriptContext;
}

// gets allocated and constructed right before the lua_State in memory
class RobloxExtraSpace
{
public:
	class Shared
	{
	private:
		char padding1[4];
	public:
		RBX::ScriptContext* scriptContext;
	} * shared;

private:
	char padding1[4];

public:
	int identity : 5;

	static inline RobloxExtraSpace* get(lua_State* L)
	{
		constexpr size_t sizeof_RobloxExtraSpace = 32;

		return reinterpret_cast<RobloxExtraSpace*>(reinterpret_cast<DWORD>(L) - sizeof_RobloxExtraSpace);
	}
};
