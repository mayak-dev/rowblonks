#include "pch.h"

#include "rowblonks.h"
#include "Patches.h"

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

        Patches::initializeHooks();
    }

    return TRUE;
}
