#pragma once

#include "LuaDefs.h"
#include "VC90Defs.h"

namespace RBX
{
	// ===== `Instance` class =====

	class Instance;

	const auto Instance__setParent = reinterpret_cast<void(__thiscall*)(Instance* _this, Instance* newParent)>(0x00597910);
	const auto Instance__setName = reinterpret_cast<void(__thiscall*)(Instance* _this, vc90::std::string* name)>(0x00598250);

	// ===== `DataModel` class =====

	class DataModel;

	const auto DataModel__find__ScriptContext = reinterpret_cast<class ScriptContext* (__thiscall*)(DataModel* _this)>(0x00468DA0);

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

	const auto ScriptContext__addScript = reinterpret_cast<void(__thiscall*)(ScriptContext* _this, void* script)>(0x006282B0);

	// ===== `Script` class =====

	class Script;

	const auto Script__constructor = reinterpret_cast<Script* (__thiscall*)(Script* _this)>(0x0063D6B0);

	// constructor helper
	inline Script* create_Script()
	{
		constexpr size_t sizeof_Script = 240;

		auto result = reinterpret_cast<Script*>((*vc90::operator_new)(sizeof_Script));
		Script__constructor(result);

		return result;
	}

	const auto Script__setDisabled = reinterpret_cast<void(__thiscall*)(Script* _this, bool disabled)>(0x0063D460);

	const auto Script__setSource = reinterpret_cast<void(__thiscall*)(Script* _this, vc90::std::string* source)>(0x0063D330);

	// ===== `CoreScript` class =====

	class CoreScript;

	const auto CoreScript__constructor = reinterpret_cast<CoreScript* (__thiscall*)(CoreScript*, vc90::std::string* source)>(0x00663570);

	// constructor helper
	inline CoreScript* create_CoreScript(const char* source)
	{
		constexpr size_t sizeof_CoreScript = 200;

		auto newSource = vc90::std::create_string(source);

		auto result = reinterpret_cast<CoreScript*>((*vc90::operator_new)(sizeof_CoreScript));
		CoreScript__constructor(result, newSource);

		(*vc90::std::string__destructor)(newSource);

		return result;
	}
}
