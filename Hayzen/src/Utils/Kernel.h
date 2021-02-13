#pragma once

namespace Kernel
{
	extern "C"
	{
		DWORD    __stdcall    ExCreateThread(
			PHANDLE                    pHandle, 
			DWORD                    dwStackSize, 
			LPDWORD                    lpThreadId, 
			VOID*                    apiThreadStartup , 
			LPTHREAD_START_ROUTINE    lpStartAddress, 
			LPVOID                    lpParameter, 
			DWORD                    dwCreationFlagsMod
			);

		VOID    __cdecl        XapiThreadStartup(
			VOID    (__cdecl *StartRoutine)(VOID *), 
			VOID    *StartContext
			);

		DWORD XamGetCurrentTitleId();
	}
}