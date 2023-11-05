#pragma once

#include "LuaDefs.h"
#include "VC90Defs.h"

namespace RBX
{
	// ===== `ScriptContext` class =====

	class ScriptContext
	{
	private:
		char padding1[152];
	public:
		lua_State* globalState;
	};

	// HOOK
	typedef void(__thiscall* ScriptContext__openState_t)(ScriptContext* _this);
	extern ScriptContext__openState_t ScriptContext__openState;

	// ===== `CoreScript` class =====

	class CoreScript;

	const auto CoreScript__constructor = reinterpret_cast<CoreScript* (__thiscall*)(CoreScript*, vc90::std::string* source)>(0x00663570);

	// constructor helper
	inline CoreScript* create_CoreScript(const char* source)
	{
		constexpr size_t sizeof_CoreScript = 200;

		auto newSource = vc90::std::create_string(source);

		CoreScript* result = reinterpret_cast<CoreScript*>(vc90::operator_new(sizeof_CoreScript));
		CoreScript__constructor(result, newSource);

		vc90::std::string__destructor(newSource);

		return result;
	}
}
