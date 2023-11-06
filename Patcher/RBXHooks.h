#pragma once

#include "RBXDefs.h"

// ===== `RBX::ContentProvider` member function hooks =====

void __cdecl RBX__ContentProvider__verifyScriptSignature_hook(vc90::std::string* source, bool required);
void __cdecl RBX__ContentProvider__verifyRequestedScriptSignature_hook(vc90::std::string* source, vc90::std::string* assetId, bool required);

// ===== `RBX:Http` member function hooks =====

RBX::Http* __fastcall RBX__Http__constructor_hook(RBX::Http* _this, void*, vc90::std::string* url);
bool __cdecl RBX__Http__trustCheck_hook(const char* url);

// ===== `RBX::DataModel` member function hooks =====

void __fastcall RBX__DataModel__startCoreScripts_hook(RBX::DataModel* _this, void*, RBX::AdornRbxGfx* adorn);

// ===== `RBX::ScriptContext` member function hooks =====

void __fastcall RBX__ScriptContext__openState_hook(RBX::ScriptContext* _this);
