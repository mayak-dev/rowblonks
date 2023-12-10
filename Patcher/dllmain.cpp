#include "pch.h"

#include "Config.h"
#include "Patches.h"

// imported functions will be patched to include this, forcing the DLL to be loaded
void __declspec(dllexport) nothing() {}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        Config::getGameDirectory(hModule);
        Config::read();

        try
        {
            Patches::initialize();
        }
        catch (const std::runtime_error& error)
        {
            (void)error;

#ifdef _DEBUG
            MessageBox(nullptr, error.what(), nullptr, MB_OK | MB_ICONERROR);
#endif

            ExitProcess(EXIT_FAILURE);
        }
    }

    return TRUE;
}
