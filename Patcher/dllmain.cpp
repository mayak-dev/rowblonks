#include "pch.h"

#include <detours.h>

#include "rowblonks.h"
#include "RBXHooks.h"

// imported functions will be patched to include this, forcing the DLL to be loaded
void __declspec(dllexport) nothing() {}

std::string g_gamePath;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        g_gamePath.resize(MAX_PATH);
        GetModuleFileName(hModule, &g_gamePath[0], MAX_PATH);
        g_gamePath.resize(g_gamePath.rfind('\\'));

        DetourTransactionBegin();

        DetourAttach((void**)&RBX::ContentProvider__verifyScriptSignature, RBX__ContentProvider__verifyScriptSignature_hook);
        DetourAttach((void**)&RBX::ContentProvider__verifyRequestedScriptSignature, RBX__ContentProvider__verifyRequestedScriptSignature_hook);

        DetourAttach((void**)&RBX::Http__constructor, RBX__Http__constructor_hook);
        DetourAttach((void**)&RBX::Http__trustCheck, RBX__Http__trustCheck_hook);

        DetourAttach((void**)&RBX::DataModel__startCoreScripts, RBX__DataModel__startCoreScripts_hook);

        DetourAttach((void**)&RBX::ScriptContext__openState, RBX__ScriptContext__openState_hook);

        DetourTransactionCommit();
    }

    return TRUE;
}
