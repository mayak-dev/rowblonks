#pragma once

#include "LuaDefs.h"
#include "VC90Defs.h"

namespace RBX
{
	// ===== `Instance` class =====

	class Instance
	{
	private:
		char padding1[68];
	public:
		Instance* parent;
	};

	const auto Instance__setParent = reinterpret_cast<void(__thiscall*)(Instance* _this, Instance* newParent)>(0x00597910);

	const auto Instance__setName = reinterpret_cast<void(__thiscall*)(Instance* _this, vc90::std::string* name)>(0x00598250);

	const auto Instance__setRobloxLocked = reinterpret_cast<void(__thiscall*)(Instance* _this, bool robloxLocked)>(0x005982F0);

	// ===== `GuiBuilder` class =====

	class GuiBuilder
	{
	public:
		class DataModel* dataModel;
		class Workspace* workspace;
	};

	const auto GuiBuilder__buildGui = reinterpret_cast<void(__thiscall*)(GuiBuilder* _this, class AdornRbxGfx* adorn, DataModel* dataModel, Workspace* workspace)>(0x007584F0);

	// ===== `DataModel` class =====

	class DataModel
	{
	private:
		char padding1[2764];
	public:
		class Workspace* workspace;
	};

	const auto DataModel__find__ScriptContext = reinterpret_cast<class ScriptContext* (__thiscall*)(DataModel* _this)>(0x00468DA0);

	// HOOK
	typedef void(__thiscall* DataModel__startCoreScripts_t)(DataModel* _this, AdornRbxGfx* adorn);
	extern DataModel__startCoreScripts_t DataModel__startCoreScripts;

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

	const auto ScriptContext__executeInNewThread = reinterpret_cast<void(__thiscall*)(ScriptContext* _this, int identity, const char* source, const char* name)>(0x00629A00);

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

	// ===== `StarterScript` class =====

	class StarterScript;

	const auto StarterScript__constructor = reinterpret_cast<StarterScript* (__thiscall*)(StarterScript*, vc90::std::string * source)>(0x00663C50);

	// constructor helper
	inline StarterScript* create_StarterScript(const char* source)
	{
		constexpr size_t sizeof_StarterScript = 200;

		auto newSource = vc90::std::create_string(source);

		auto result = reinterpret_cast<StarterScript*>((*vc90::operator_new)(sizeof_StarterScript));
		StarterScript__constructor(result, newSource);

		(*vc90::std::string__destructor)(newSource);

		return result;
	}

	// ===== `ContentProvider` class =====

	class ContentProvider;

	// static method
	// HOOK
	typedef void(__cdecl* ContentProvider__verifyScriptSignature_t)(vc90::std::string* source, bool required);
	extern ContentProvider__verifyScriptSignature_t ContentProvider__verifyScriptSignature;

	// static method
	// HOOK
	typedef void(__cdecl* ContentProvider__verifyRequestedScriptSignature_t)(vc90::std::string* source, vc90::std::string* assetId, bool required);
	extern ContentProvider__verifyRequestedScriptSignature_t ContentProvider__verifyRequestedScriptSignature;

	// ===== `Http` class =====

	class Http;

	// HOOK
	typedef Http* (__thiscall* Http__constructor_t)(Http* _this, vc90::std::string* url);
	extern Http__constructor_t Http__constructor;

	// static method
	// HOOK
	typedef bool(__cdecl* Http__trustCheck_t)(const char* url);
	extern Http__trustCheck_t Http__trustCheck;

	namespace Network
	{
		// ===== `Replicator::RockyItem` class =====

		class Replicator__RockyItem;

		// HOOK
		typedef bool(__thiscall* Replicator__RockyItem__write_t)(Replicator__RockyItem* _this, void* bitStream);
		extern Replicator__RockyItem__write_t Replicator__RockyItem__write;
	}
}
