#pragma once

#include "RBXDefs.h"

// ===== `CRobloxWnd::RenderRequestJob` member function hooks =====

double* __fastcall CRobloxWnd__RenderRequestJob__sleepTime_hook(CRobloxWnd__RenderRequestJob* _this, void*, double* a2, int a3);

// ===== `CRobloxWnd::UserInputJob` member function hooks =====

double* __fastcall CRobloxWnd__UserInputJob__sleepTime_hook(CRobloxWnd__UserInputJob* _this, void*, double* a2, int a3);

namespace RBX
{
	// ===== `RBX::HeartbeatTask` member function hooks ====

	RBX::HeartbeatTask* __fastcall HeartbeatTask__constructor_hook(RBX::HeartbeatTask* _this, void*, RBX::RunService* runService, void* a3);

	// ===== `RBX::ContentProvider` member function hooks =====

	// SECURITY BYPASS
	void __cdecl ContentProvider__verifyScriptSignature_hook(vc90::std::string* source, bool required);

	// SECURITY BYPASS
	void __cdecl ContentProvider__verifyRequestedScriptSignature_hook(vc90::std::string* source, vc90::std::string* assetId, bool required);

	// ===== `RBX:Http` member function hooks =====

	RBX::Http* __fastcall Http__constructor_hook(RBX::Http* _this, void*, vc90::std::string* url);
	bool __cdecl Http__trustCheck_hook(const char* url);

	// ===== `RBX::DataModel` member function hooks =====

	void __fastcall DataModel__startCoreScripts_hook(RBX::DataModel* _this, void*, RBX::AdornRbxGfx* adorn);

	// ===== `RBX::ScriptContext` member function hooks =====

	void __fastcall ScriptContext__openState_hook(RBX::ScriptContext* _this);
	void __fastcall ScriptContext__executeInNewThread_hook(RBX::ScriptContext* _this, void*, int identity, const char* source, const char* name);

	// ===== `RBX::Network::Replicator::RockyItem` member function hooks =====

	// SECURITY BYPASS
	bool __fastcall Network__Replicator__RockyItem__write_hook(RBX::Network::Replicator__RockyItem* _this, void*, void* bitStream);

	// ===== `RBX::PlayerChatLine` member function hooks =====

	PlayerChatLine* __fastcall PlayerChatLine__constructor_hook(PlayerChatLine* _this, void*, int a2, RBX::Player* player, void* a4, int a5, int a6, int a7);

	// ===== `RBX::NetworkSettings` member function hooks =====

	float __fastcall NetworkSettings__getDataSendRate_hook(RBX::NetworkSettings* _this);
	double __fastcall NetworkSettings__getReceiveRate_hook(RBX::NetworkSettings* _this);
}
