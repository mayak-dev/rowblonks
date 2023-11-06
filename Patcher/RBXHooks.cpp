#include "pch.h"
#include "RBXHooks.h"
#include "LuaApiExtensions.h"

// ===== `RBX::ScriptContext` member function hooks =====

RBX::ScriptContext__openState_t RBX::ScriptContext__openState = reinterpret_cast<RBX::ScriptContext__openState_t>(0x00625BF0);

void __fastcall RBX__ScriptContext__openState_hook(RBX::ScriptContext* _this)
{
	if (!_this->globalState)
	{
		RBX::ScriptContext__openState(_this);

		// globalState should no longer be null at this point

		if (_this->globalState)
		{
			lua_pushcfunction(_this->globalState, Lua::addLocalCoreScript);
			lua_setglobal(_this->globalState, "AddLocalCoreScript");

			lua_pushcfunction(_this->globalState, Lua::registerLocalLibrary);
			lua_setglobal(_this->globalState, "RegisterLocalLibrary");
		}
	}
}
