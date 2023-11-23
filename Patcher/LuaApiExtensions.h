#pragma once

struct lua_State;

namespace Lua
{
	// ===== functionality for global/game chat messages =====

	int produceGameChat(lua_State* L);

	// ===== functionality for local corescripts and libraries =====

	int addLocalCoreScript(lua_State* L);
	int addLocalStarterScript(lua_State* L);
	int registerLocalLibrary(lua_State* L);
}
