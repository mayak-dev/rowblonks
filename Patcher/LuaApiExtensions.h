#pragma once

struct lua_State;

namespace Lua
{
	int addLocalCoreScript(lua_State* L);
	int addLocalStarterScript(lua_State* L);
	int registerLocalLibrary(lua_State* L);
}
