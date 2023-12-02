#pragma once

#include "RBXDefs.h"

// ===== `CRobloxWnd::RenderRequestJob` member function hooks =====

double* __fastcall hook_CRobloxWnd__RenderRequestJob__sleepTime(CRobloxWnd__RenderRequestJob* _this, void*, double* a2, int a3);

// ===== `CRobloxWnd::UserInputJob` member function hooks =====

double* __fastcall hook_CRobloxWnd__UserInputJob__sleepTime(CRobloxWnd__UserInputJob* _this, void*, double* a2, int a3);

// ===== `RBX::HeartbeatTask` member function hooks ====

RBX::HeartbeatTask* __fastcall hook_RBX__HeartbeatTask__constructor(RBX::HeartbeatTask* _this, void*, RBX::RunService* runService, void* a3);

// ===== `RBX::ContentProvider` member function hooks =====

// SECURITY BYPASS
void __cdecl hook_RBX__ContentProvider__verifyScriptSignature(vc90::std::string* source, bool required);

// SECURITY BYPASS
void __cdecl RBX__ContentProvider__verifyRequestedScriptSignature_hook(vc90::std::string* source, vc90::std::string* assetId, bool required);

// ===== `RBX:Http` member function hooks =====

RBX::Http* __fastcall hook_RBX__Http__constructor(RBX::Http* _this, void*, vc90::std::string* url);
bool __cdecl hook_RBX__Http__trustCheck(const char* url);

// ===== `RBX::DataModel` member function hooks =====

void __fastcall hook_RBX__DataModel__startCoreScripts(RBX::DataModel* _this, void*, RBX::AdornRbxGfx* adorn);

// ===== `RBX::ScriptContext` member function hooks =====

void __fastcall hook_RBX__ScriptContext__openState(RBX::ScriptContext* _this);

// ===== `RBX::Network::Replicator::RockyItem` member function hooks =====

// SECURITY BYPASS
bool __fastcall hook_RBX__Network__Replicator__RockyItem__write(RBX::Network::Replicator__RockyItem* _this, void*, void* bitStream);