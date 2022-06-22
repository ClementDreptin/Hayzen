#include "pch.h"
#include "Core\Plugin.h"


// The application's entry point.
BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, void *pReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
        Plugin::Start();

    if (dwReason == DLL_PROCESS_DETACH)
        Plugin::Stop();

    return TRUE;
}
