#pragma once

struct lua_State;

typedef int (*lua_CFunction)(lua_State* L);

namespace RBX
{
	class Instance;
	class ScriptContext;
	class DataModel;
}

namespace Lua
{
	RBX::Instance* checkInstance(lua_State* L, int n, void* downcastTypeDesc = nullptr, const char* className = nullptr);

	std::pair<RBX::ScriptContext*, RBX::DataModel*> getScriptContextAndDataModel(lua_State* L);

	void checkPermissions(lua_State* L, int role, const char* action);

	void protectLibrary(lua_State* L, const char* name);
	void openProtectedLibrary(lua_State* L, const char* name, lua_CFunction func);
}
