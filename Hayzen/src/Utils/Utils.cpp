#include "pch.h"
#include "Utils\Utils.h"

#include "Utils\Formater.h"

DWORD Utils::ResolveFunction(const std::string& moduleName, DWORD ordinal)
{
	HMODULE mHandle = GetModuleHandle(moduleName.c_str());

	return (mHandle == NULL) ? NULL : (DWORD)GetProcAddress(mHandle, (LPCSTR)ordinal);
}

void Utils::Thread(LPTHREAD_START_ROUTINE lpStartAddress)
{
	HANDLE hThread;
	DWORD dwThreadId;
	ExCreateThread(&hThread, 0, &dwThreadId, (PVOID)XapiThreadStartup, lpStartAddress, 0, 2 | CREATE_SUSPENDED);
	XSetThreadProcessor(hThread, 4);
	SetThreadPriority(hThread, THREAD_PRIORITY_NORMAL);
	ResumeThread(hThread);
}

void Utils::PatchInJump(DWORD* address, DWORD destination, BOOL linked)
{
    if(destination & 0x8000)
		address[0] = 0x3D600000 + (((destination >> 16) & 0xFFFF) + 1);
    else
		address[0] = 0x3D600000 + ((destination >> 16) & 0xFFFF);

    address[1] = 0x396B0000 + (destination & 0xFFFF);
    address[2] = 0x7D6903A6;

    if(linked)
		address[3] = 0x4E800421;
    else
		address[3] = 0x4E800420;
}

void Utils::XNotify(const std::string& text, XNOTIFYQUEUEUI_TYPE type)
{
	XNotifyQueueUI(type, 0xFF, XNOTIFY_SYSTEM, Formater::CharToWChar(text.c_str()), 0);
}