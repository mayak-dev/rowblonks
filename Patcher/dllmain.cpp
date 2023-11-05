#include "pch.h"

#include <detours.h>

#include "RBXHooks.h"

// imported functions will be patched to include this, forcing the DLL to be loaded
void __declspec(dllexport) nothing() {}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        DetourTransactionBegin();

        DetourAttach((void**)&RBX::ScriptContext__openState, RBX__ScriptContext__openState_hook);

        DetourTransactionCommit();
    }

    return TRUE;
}
