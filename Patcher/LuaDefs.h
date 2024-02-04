#pragma once

#include "LuaHelpers.h"

struct lua_State;

typedef int (*lua_CFunction)(lua_State* L);

typedef struct luaL_Reg {
	const char* name;
	lua_CFunction func;
} luaL_Reg;

typedef double lua_Number;

const auto lua_pushcclosure = reinterpret_cast<void(__cdecl*)(lua_State* L, lua_CFunction fn, int n)>(0x00762A70);
const auto lua_pushinteger = reinterpret_cast<void(__cdecl*)(lua_State* L, int n)>(0x00762940);
const auto lua_pushstring = reinterpret_cast<void(__cdecl*)(lua_State* L, const char* s)>(0x007629A0);
const auto lua_pushlstring = reinterpret_cast<void(__cdecl*)(lua_State* L, const char* s, size_t len)>(0x00762960);
const auto lua_pushnumber = reinterpret_cast<void(__cdecl*)(lua_State* L, lua_Number n)>(0x00762920);

const auto lua_setfield = reinterpret_cast<void(__cdecl*)(lua_State* L, int idx, const char* k)>(0x00762DF0);
const auto lua_getfield = reinterpret_cast<void(__cdecl*)(lua_State* L, int idx, const char* k)>(0x00762BB0);

const auto lua_settop = reinterpret_cast<void(__cdecl*)(lua_State* L, int idx)>(0x00762370);
const auto lua_gettop = reinterpret_cast<int(__cdecl*)(lua_State* L)>(0x00762360);

const auto luaL_checklstring = reinterpret_cast<const char* (__cdecl*)(lua_State* L, int narg, size_t* len)>(0x00764190);
const auto luaL_checkudata = reinterpret_cast<void* (__cdecl*)(lua_State* L, int ud, const char* tname)>(0x00764080);
const auto luaL_checkinteger = reinterpret_cast<int(__cdecl*)(lua_State* L, int narg)>(0x007642D0);
const auto luaL_checknumber = reinterpret_cast<lua_Number(__cdecl*)(lua_State* L, int narg)>(0x00764250);

const auto lua_isnumber = reinterpret_cast<int(__cdecl*)(lua_State* L, int idx)>(0x007625C0);

const auto lua_tonumber = reinterpret_cast<lua_Number(__cdecl*)(lua_State* L, int idx)>(0x007626B0);

const auto luaL_error = reinterpret_cast<int(__cdecl*)(lua_State* L, const char* fmt, ...)>(0x00763710);

const auto luaL_register = reinterpret_cast<void(__cdecl*)(lua_State* L, const char* libname, const luaL_Reg* l)>(0x00764540);

const auto lua_newuserdata = reinterpret_cast<void(__cdecl*)(lua_State* L, size_t size)>(0x007633B0);

const auto lua_setmetatable = reinterpret_cast<int(__cdecl*)(lua_State* L, int objindex)>(0x00762F40);

const auto lua_createtable = reinterpret_cast<void(__cdecl*)(lua_State* L, int narray, int nrec)>(0x00762C90);

const auto lua_type = reinterpret_cast<int(__cdecl*)(lua_State* L, int idx)>(0x00762550);

#define LUA_REGISTRYINDEX	(-10000)
#define LUA_ENVIRONINDEX	(-10001)
#define LUA_GLOBALSINDEX	(-10002)

#define LUA_TNONE               (-1)
#define LUA_TNIL                0
#define LUA_TBOOLEAN            1
#define LUA_TLIGHTUSERDATA      2
#define LUA_TNUMBER             3
#define LUA_TSTRING             4
#define LUA_TTABLE              5
#define LUA_TFUNCTION           6
#define LUA_TUSERDATA           7
#define LUA_TTHREAD             8

#define lua_pushcfunction(L,f)  lua_pushcclosure(L, (f), 0)

#define lua_setglobal(L,s)      lua_setfield(L, LUA_GLOBALSINDEX, (s))
#define lua_getglobal(L,s)      lua_getfield(L, LUA_GLOBALSINDEX, (s))

#define luaL_checkstring(L,n)   (luaL_checklstring(L, (n), NULL))
#define luaL_checkint(L,n)      ((int)luaL_checkinteger(L, (n)))

#define lua_pop(L,n)            lua_settop(L, -(n)-1)

#define lua_newtable(L)         lua_createtable(L, 0, 0)

#define lua_isnone(L,n)         (lua_type(L, (n)) == LUA_TNONE)

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
