#include "pch.h"
#include "Core\Plugin.h"


//-------------------------------------------------------------------------------------
// Name: DllMain()
// Desc: The application's entry point.
//-------------------------------------------------------------------------------------
BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
        Plugin::Start();

    if (dwReason == DLL_PROCESS_DETACH)
        Plugin::Stop();

    return TRUE;
}