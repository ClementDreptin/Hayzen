#include "pch.h"
#include "Core/Plugin.h"

Plugin *g_pPlugin = nullptr;
HANDLE g_SystemThreadHandle = INVALID_HANDLE_VALUE;

DWORD WINAPI Attach(HANDLE hModule)
{
    g_pPlugin = new Plugin(hModule);

    HRESULT hr = g_pPlugin->Init();
    if (FAILED(hr))
        return hr;

    g_pPlugin->Run();

    return 0;
}

void Detach()
{
    delete g_pPlugin;

    WaitForSingleObject(g_SystemThreadHandle, INFINITE);
    CloseHandle(g_SystemThreadHandle);
}

int DllMain(HANDLE hModule, DWORD reason, void *pReserved)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        // We run the entrypoint (Attach) in a system thread so that it is not terminated
        // when another game is launched
        g_SystemThreadHandle = ThreadEx(Attach, hModule, EXCREATETHREAD_FLAG_SYSTEM);
    }

    if (reason == DLL_PROCESS_DETACH)
        Detach();

    return TRUE;
}
