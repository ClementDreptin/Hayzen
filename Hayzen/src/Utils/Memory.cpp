#include "pch.h"
#include "Utils\Memory.h"

void Memory::Thread(LPTHREAD_START_ROUTINE lpStartAddress)
{
	HANDLE hThread;
	DWORD dwThreadId;
	ExCreateThread(&hThread, 0, &dwThreadId, (PVOID)XapiThreadStartup, lpStartAddress, 0, 2 | CREATE_SUSPENDED);
	XSetThreadProcessor(hThread, 4);
	SetThreadPriority(hThread, THREAD_PRIORITY_NORMAL);
	ResumeThread(hThread);
}