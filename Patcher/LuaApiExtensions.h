#pragma once

struct lua_State;

namespace Lua
{
	int openApiExtensionsLibrary(lua_State* L);

	namespace Api
	{
		// ===== functionality related to script identities =====

		int getIdentity(lua_State* L);
		int spoofIdentity(lua_State* L);

		// ===== functionality for global/game chat messages =====

		int produceGameChat(lua_State* L);

		// ===== functionality for local corescripts and libraries =====

		int addLocalCoreScript(lua_State* L);
		int addLocalStarterScript(lua_State* L);
		int registerLocalLibrary(lua_State* L);

		// ===== functionality to show an IDE window =====

		int showIdeWindow(lua_State* L);
	}
}

// BitOp library
int luaopen_bit(lua_State* L);
