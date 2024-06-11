#pragma once

#include "RBXDefs.h"

// ===== `CRobloxWnd::RenderRequestJob` member function hooks =====

double* __fastcall CRobloxWnd__RenderRequestJob__sleepTime_hook(CRobloxWnd__RenderRequestJob* _this, void*, double* a2, int a3);

// ===== `CRobloxWnd::UserInputJob` member function hooks =====

double* __fastcall CRobloxWnd__UserInputJob__sleepTime_hook(CRobloxWnd__UserInputJob* _this, void*, double* a2, int a3);

namespace RBX
{
	// ===== `RBX::HeartbeatTask` member function hooks =====

	RBX::HeartbeatTask* __fastcall HeartbeatTask__constructor_hook(RBX::HeartbeatTask* _this, void*, RBX::RunService* runService, void* a3);

	// ===== `RBX::PhysicsJob` member function hooks =====

	RBX::PhysicsJob* __fastcall PhysicsJob__constructor_hook(RBX::PhysicsJob* _this, void*, RBX::DataModel* dataModel, void* a3);

	// ===== `RBX::ContentProvider` member function hooks =====

	// SECURITY BYPASS
	void __cdecl ContentProvider__verifyScriptSignature_hook(const std::string& source, bool required);

	// SECURITY BYPASS
	void __cdecl ContentProvider__verifyRequestedScriptSignature_hook(const std::string& source, const std::string& assetId, bool required);

	// ===== `RBX:Http` member function hooks =====

	RBX::Http* __fastcall Http__constructor_hook(RBX::Http* _this, void*, const std::string& url);
	bool __cdecl Http__trustCheck_hook(const char* url);

	// ===== `RBX::DataModel` member function hooks =====

	void __fastcall DataModel__startCoreScripts_hook(RBX::DataModel* _this, void*, RBX::AdornRbxGfx* adorn);
	void __fastcall DataModel__physicsStep_hook(RBX::DataModel* _this, void*, float a2, double a3, double a4);

	// ===== `RBX::ScriptContext` member function hooks =====

	void __fastcall ScriptContext__openState_hook(RBX::ScriptContext* _this);
	void __fastcall ScriptContext__executeInNewThread_hook(RBX::ScriptContext* _this, void*, int identity, const char* source, const char* name);

	// ===== `RBX::Network::Replicator::RockyItem` member function hooks =====

	// SECURITY BYPASS
	bool __fastcall Network__Replicator__RockyItem__write_hook(RBX::Network::Replicator__RockyItem* _this, void*, void* bitStream);

	// ===== `RBX::PlayerChatLine` member function hooks =====

	PlayerChatLine* __fastcall PlayerChatLine__constructor_hook(PlayerChatLine* _this, void*, int a2, RBX::Player* player, void* a4, int a5, int a6, int a7);

	// ===== `RBX::NetworkSettings` member function hooks =====

	void __fastcall NetworkSettings__setDataSendRate_hook(RBX::NetworkSettings* _this, void*, float dataSendRate);
	void __fastcall NetworkSettings__setReceiveRate_hook(RBX::NetworkSettings* _this, void*, double receiveRate);
	void __fastcall NetworkSettings__setPhysicsSendRate_hook(RBX::NetworkSettings* _this, void*, float physicsSendRate);

	// ===== `RBX::VideoControl` member function hooks =====

	bool __fastcall VideoControl__isVideoRecording_hook(RBX::VideoControl* _this);

	// ===== `RBX::RunService` member function hooks =====

	void __fastcall RunService__step_hook(RBX::RunService* _this, void*, double a2);
}
