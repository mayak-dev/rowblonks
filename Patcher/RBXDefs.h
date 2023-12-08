#pragma once

#include "LuaDefs.h"
#include "VC90Defs.h"

// ===== `CRobloxWnd::RenderRequestJob` class =====

class CRobloxWnd__RenderRequestJob
{
private:
	char padding1[480];
public:
	bool awake;
};

// HOOK
typedef double* (__thiscall* CRobloxWnd__RenderRequestJob__sleepTime_t)(CRobloxWnd__RenderRequestJob* _this, double* a2, int a3);
extern CRobloxWnd__RenderRequestJob__sleepTime_t CRobloxWnd__RenderRequestJob__sleepTime;

// ===== `CRobloxWnd::UserInputJob` class =====

class CRobloxWnd__UserInputJob;

// HOOK
typedef double* (__thiscall* CRobloxWnd__UserInputJob__sleepTime_t)(CRobloxWnd__UserInputJob* _this, double* a2, int a3);
extern CRobloxWnd__UserInputJob__sleepTime_t CRobloxWnd__UserInputJob__sleepTime;

namespace RBX
{
	namespace Network
	{
		// ===== `Server` class =====

		class Server;

		// ===== `Replicator::RockyItem` class =====

		class Replicator__RockyItem;

		// HOOK
		typedef bool(__thiscall* Replicator__RockyItem__write_t)(Replicator__RockyItem* _this, void* bitStream);
		extern Replicator__RockyItem__write_t Replicator__RockyItem__write;
	}

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
		Workspace* workspace;
	};

	const auto DataModel__find__ScriptContext = reinterpret_cast<class ScriptContext* (__thiscall*)(DataModel* _this)>(0x00468DA0);
	const auto DataModel__find__Players = reinterpret_cast<class Players* (__thiscall*)(DataModel* _this)>(0x00414A30);
	const auto DataModel__find__Network__Server = reinterpret_cast<Network::Server* (__thiscall*)(DataModel* _this)>(0x004D1150);

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

	// ===== `Players` class =====

	class Players;

	const auto Players__gameChat = reinterpret_cast<void(__thiscall*)(Players* _this, vc90::std::string* message)>(0x004CAE90);

	// ===== `Script` class =====

	const auto Script__constructor = reinterpret_cast<class Script* (__thiscall*)(Script* _this) > (0x0063D6B0);

	class Script
	{
	public:
		static constexpr size_t size = 240;
		
		// constructor helper
		static Script* construct()
		{
			auto result = reinterpret_cast<Script*>((*vc90::operator_new)(size));
			Script__constructor(result);

			return result;
		}
	};

	const auto Script__setDisabled = reinterpret_cast<void(__thiscall*)(Script* _this, bool disabled)>(0x0063D460);

	const auto Script__setSource = reinterpret_cast<void(__thiscall*)(Script* _this, vc90::std::string* source)>(0x0063D330);

	// ===== `CoreScript` class =====

	const auto CoreScript__constructor = reinterpret_cast<class CoreScript* (__thiscall*)(CoreScript*, vc90::std::string* source)>(0x00663570);

	class CoreScript
	{
	public:
		static constexpr size_t size = 200;

		// constructor helper
		static CoreScript* construct(const char* source)
		{
			auto newSource = vc90::std::string::construct(source);

			auto result = reinterpret_cast<CoreScript*>((*vc90::operator_new)(size));
			CoreScript__constructor(result, newSource);

			vc90::std::string::destruct(newSource);

			return result;
		}
	};

	// ===== `StarterScript` class =====

	const auto StarterScript__constructor = reinterpret_cast<class StarterScript* (__thiscall*)(StarterScript*, vc90::std::string* source)>(0x00663C50);

	class StarterScript
	{
	public:
		static constexpr size_t size = 200;

		// constructor helper
		static StarterScript* construct(const char* source)
		{
			auto newSource = vc90::std::string::construct(source);

			auto result = reinterpret_cast<StarterScript*>((*vc90::operator_new)(size));
			StarterScript__constructor(result, newSource);

			vc90::std::string::destruct(newSource);

			return result;
		}
	};

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

	// ===== `RunService` class =====

	class RunService;

	// ===== `HeartbeatTask` class =====

	class HeartbeatTask
	{
	private:
		char padding1[496];
	public:
		double fps;
	};

	// HOOK
	typedef HeartbeatTask* (__thiscall* HeartbeatTask__constructor_t)(HeartbeatTask* _this, RunService* runService, void* a3);
	extern HeartbeatTask__constructor_t HeartbeatTask__constructor;
}
