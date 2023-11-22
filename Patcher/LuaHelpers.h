#pragma once

struct lua_State;

namespace RBX
{
	class Instance;
	class DataModel;
}

namespace Lua
{
	RBX::Instance* checkInstance(lua_State* L, int n);

	RBX::DataModel* getDataModel(lua_State* L);

	void checkIdentity(lua_State* L, int minIdentity, const char* action);
}
