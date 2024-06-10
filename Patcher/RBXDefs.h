#pragma once

#include "VC90Defs.h"

struct lua_State;

// ===== `CMainFrame` class =====

class CMainFrame;

const auto CMainFrame__ShowOutput = reinterpret_cast<void(__thiscall*)(CMainFrame* _this)>(0x00431480);
const auto CMainFrame__ShowTaskScheduler = reinterpret_cast<void(__thiscall*)(CMainFrame* _this)>(0x00430AA0);

// ===== `CRobloxApp` class =====

class CRobloxApp
{
private:
	char padding[32];
public:
	CMainFrame* m_pActiveWnd;
};

static CRobloxApp* const g_CRobloxApp = reinterpret_cast<CRobloxApp*>(0x00CB3820);

// ===== `CRobloxWnd::RenderRequestJob` class =====

class CRobloxWnd__RenderRequestJob
{
private:
	char padding1[480];
public:
	bool awake;
};

// HOOKED
typedef double* (__thiscall* CRobloxWnd__RenderRequestJob__sleepTime_t)(CRobloxWnd__RenderRequestJob* _this, double* a2, int a3);
extern CRobloxWnd__RenderRequestJob__sleepTime_t CRobloxWnd__RenderRequestJob__sleepTime_orig;

// ===== `CRobloxWnd::UserInputJob` class =====

class CRobloxWnd__UserInputJob;

// HOOKED
typedef double* (__thiscall* CRobloxWnd__UserInputJob__sleepTime_t)(CRobloxWnd__UserInputJob* _this, double* a2, int a3);
extern CRobloxWnd__UserInputJob__sleepTime_t CRobloxWnd__UserInputJob__sleepTime_orig;

namespace RBX
{
	namespace Network
	{
		// ===== `Server` class =====

		class Server;

		// ===== `Replicator::RockyItem` class =====

		class Replicator__RockyItem;

		// HOOKED
		typedef bool(__thiscall* Replicator__RockyItem__write_t)(Replicator__RockyItem* _this, void* bitStream);
		extern Replicator__RockyItem__write_t Replicator__RockyItem__write_orig;
	}

	// ===== `Instance` class =====

	class Instance
	{
	private:
		char padding1[68];
	public:
		Instance* parent;
	private:
		char padding2[8];
	public:
		void* name; // this marks the offset of a std::string, it is not a pointer to one
	};

	const auto Instance__setParent = reinterpret_cast<void(__thiscall*)(Instance* _this, Instance* newParent)>(0x00597910);

	const auto Instance__setName = reinterpret_cast<void(__thiscall*)(Instance* _this, const std::string& name)>(0x00598250);

	const auto Instance__setRobloxLocked = reinterpret_cast<void(__thiscall*)(Instance* _this, bool robloxLocked)>(0x005982F0);

	const auto Instance__raisePropertyChanged = reinterpret_cast<void(__thiscall*)(Instance* _this, void* propDescriptor)>(0x00411F60);

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

	// HOOKED
	typedef void(__thiscall* DataModel__startCoreScripts_t)(DataModel* _this, AdornRbxGfx* adorn);
	extern DataModel__startCoreScripts_t DataModel__startCoreScripts_orig;

	// HOOKED
	typedef void(__thiscall* DataModel__physicsStep_t)(DataModel* _this, float a2, double a3, double a4);
	extern DataModel__physicsStep_t DataModel__physicsStep_orig;

	// ===== `ScriptContext` class =====

	class ScriptContext
	{
	private:
		char padding1[152];
	public:
		lua_State* globalState;
	};

	// HOOKED
	typedef void(__thiscall* ScriptContext__openState_t)(ScriptContext* _this);
	extern ScriptContext__openState_t ScriptContext__openState_orig;

	const auto ScriptContext__addScript = reinterpret_cast<void(__thiscall*)(ScriptContext* _this, void* script)>(0x006282B0);

	//HOOKED
	typedef void(__thiscall* ScriptContext__executeInNewThread_t)(ScriptContext* _this, int identity, const char* source, const char* name);
	extern ScriptContext__executeInNewThread_t ScriptContext__executeInNewThread_orig;

	// ===== `Players` class =====

	class Players;

	const auto Players__gameChat = reinterpret_cast<void(__thiscall*)(Players* _this, const std::string& message)>(0x004CAE90);

	// ===== `Script` class =====

	const auto Script__constructor = reinterpret_cast<class Script* (__thiscall*)(Script* _this)>(0x0063D6B0);

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

	const auto Script__setSource = reinterpret_cast<void(__thiscall*)(Script* _this, const std::string& source)>(0x0063D330);

	// ===== `CoreScript` class =====

	const auto CoreScript__constructor = reinterpret_cast<class CoreScript* (__thiscall*)(CoreScript*, const std::string& source)>(0x00663570);

	class CoreScript
	{
	public:
		static constexpr size_t size = 200;

		// constructor helper
		static CoreScript* construct(const std::string& source)
		{
			auto result = reinterpret_cast<CoreScript*>((*vc90::operator_new)(size));
			CoreScript__constructor(result, source);
			return result;
		}
	};

	// ===== `StarterScript` class =====

	const auto StarterScript__constructor = reinterpret_cast<class StarterScript* (__thiscall*)(StarterScript*, const std::string& source)>(0x00663C50);

	class StarterScript
	{
	public:
		static constexpr size_t size = 200;

		// constructor helper
		static StarterScript* construct(const std::string& source)
		{
			auto result = reinterpret_cast<StarterScript*>((*vc90::operator_new)(size));
			StarterScript__constructor(result, source);

			return result;
		}
	};

	// ===== `ContentProvider` class =====

	class ContentProvider;

	// static method
	// HOOKED
	typedef void(__cdecl* ContentProvider__verifyScriptSignature_t)(const std::string& source, bool required);
	extern ContentProvider__verifyScriptSignature_t ContentProvider__verifyScriptSignature_orig;

	// static method
	// HOOKED
	typedef void(__cdecl* ContentProvider__verifyRequestedScriptSignature_t)(const std::string& source, const std::string& assetId, bool required);
	extern ContentProvider__verifyRequestedScriptSignature_t ContentProvider__verifyRequestedScriptSignature_orig;

	// ===== `Http` class =====

	class Http;

	// HOOKED
	typedef Http* (__thiscall* Http__constructor_t)(Http* _this, const std::string& url);
	extern Http__constructor_t Http__constructor_orig;

	// static method
	// HOOKED
	typedef bool(__cdecl* Http__trustCheck_t)(const char* url);
	extern Http__trustCheck_t Http__trustCheck_orig;

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

	// HOOKED
	typedef HeartbeatTask* (__thiscall* HeartbeatTask__constructor_t)(HeartbeatTask* _this, RunService* runService, void* a3);
	extern HeartbeatTask__constructor_t HeartbeatTask__constructor_orig;

	// ===== `PhysicsJob` class =====

	class PhysicsJob
	{
	private:
		char padding1[480];
	public:
		double fps;
	};

	// HOOKED
	typedef PhysicsJob* (__thiscall* PhysicsJob__constructor_t)(PhysicsJob* _this, DataModel* dataModel, void* a3);
	extern PhysicsJob__constructor_t PhysicsJob__constructor_orig;

	// ===== `Player` class =====

	class Player
	{
	private:
		char padding1[156];
	public:
		bool neutral;
	};

	// ===== `PlayerChatLine` class =====

	class PlayerChatLine
	{
	private:
		char padding1[60];
	public:
		float nameR;
		float nameG;
		float nameB;
		std::string name;
	};

	// HOOKED
	typedef PlayerChatLine* (__thiscall* PlayerChatLine__constructor_t)(PlayerChatLine* _this, int a2, class Player* player, void* a4, int a5, int a6, int a7);
	extern PlayerChatLine__constructor_t PlayerChatLine__constructor_orig;

	// ===== `NetworkSettings` class =====

	class NetworkSettings
	{
	private:
		char padding1[224];
	public:
		float dataSendRate;
	private:
		char padding2[4];
	public:
		double receiveRate;
	};

	// HOOKED
	typedef void(__thiscall* NetworkSettings__setDataSendRate_t)(NetworkSettings* _this, float dataSendRate);
	extern NetworkSettings__setDataSendRate_t NetworkSettings__setDataSendRate_orig;

	// HOOKED
	typedef void(__thiscall* NetworkSettings__setReceiveRate_t)(NetworkSettings* _this, double receiveRate);
	extern NetworkSettings__setReceiveRate_t NetworkSettings__setReceiveRate_orig;

	// ===== `LuaSettings` class =====

	class LuaSettings
	{
	private:
		char padding1[160];
	public:
		double defaultWaitTime;

		// singleton helper
		static LuaSettings* singleton()
		{
			return *reinterpret_cast<LuaSettings**>(0x00CCBF7C);
		}
	};

	// ===== `VideoControl` class =====

	class VideoControl;

	// HOOKED
	typedef bool(__thiscall* VideoControl__isVideoRecording_t)(VideoControl* _this);
	extern VideoControl__isVideoRecording_t VideoControl__isVideoRecording_orig;

	// ==== `StandardOut` class =====

	class StandardOut
	{
	public:

		// singleton helper
		static StandardOut* singleton()
		{
			return *reinterpret_cast<StandardOut**>(0x00CBDF3C);
		}
	};

	enum MessageType
	{
		MESSAGE_OUTPUT,
		MESSAGE_INFO,
		MESSAGE_WARNING,
		MESSAGE_ERROR
	};

	const auto StandardOut__print = reinterpret_cast<void(__thiscall*)(StandardOut* _this, MessageType messageType, const std::string& message)>(0x005B17E0);
}
