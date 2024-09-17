#include "pch.h"
#include "Core/Plugin.h"

Plugin *g_pPlugin = nullptr;

int DllMain(HANDLE hModule, DWORD reason, void *pReserved)
{
    if (reason == DLL_PROCESS_ATTACH)
        g_pPlugin = new Plugin(hModule);

    if (reason == DLL_PROCESS_DETACH)
        delete g_pPlugin;

    return TRUE;
}
