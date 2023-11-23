#pragma once

struct lua_State;

namespace RBX
{
	class Instance;
	class ScriptContext;
	class DataModel;
}

namespace Lua
{
	RBX::Instance* checkInstance(lua_State* L, int n);

	std::pair<RBX::ScriptContext*, RBX::DataModel*> getScriptContextAndDataModel(lua_State* L);

	void checkIdentity(lua_State* L, int minIdentity, const char* action);
}
