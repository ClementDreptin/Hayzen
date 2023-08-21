#include "pch.h"
#include "Core/Plugin.h"

Plugin *g_pPlugin;

BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, void *pReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
        g_pPlugin = new Plugin();

    if (dwReason == DLL_PROCESS_DETACH)
        delete g_pPlugin;

    return TRUE;
}
