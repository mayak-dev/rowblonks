#include "pch.h"
#include "Patches.h"
#include "RBXHooks.h"

#include <detours.h>

void Patches::initializeHooks()
{
    DetourTransactionBegin();

    DetourAttach((void**)&RBX::ContentProvider__verifyScriptSignature, RBX__ContentProvider__verifyScriptSignature_hook);
    DetourAttach((void**)&RBX::ContentProvider__verifyRequestedScriptSignature, RBX__ContentProvider__verifyRequestedScriptSignature_hook);

    DetourAttach((void**)&RBX::Http__constructor, RBX__Http__constructor_hook);
    DetourAttach((void**)&RBX::Http__trustCheck, RBX__Http__trustCheck_hook);

    DetourAttach((void**)&RBX::DataModel__startCoreScripts, RBX__DataModel__startCoreScripts_hook);

    DetourAttach((void**)&RBX::ScriptContext__openState, RBX__ScriptContext__openState_hook);

    DetourTransactionCommit();
}
