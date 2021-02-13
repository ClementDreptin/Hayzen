#include "pch.h"
#include "Utils\Xam.h"

void Xam::XNotifyQueueUI(XNOTIFYQUEUEUI_TYPE exnq, DWORD dwUserIndex, ULONGLONG qwAreas, PWCHAR displayText, PVOID contextData)
{
	void(__cdecl* TmpFunc)(
		XNOTIFYQUEUEUI_TYPE exnq,
		DWORD dwUserIndex,
		ULONGLONG qwAreas,
		PWCHAR displayText,
		PVOID contextData
	) = (void(__cdecl*)(XNOTIFYQUEUEUI_TYPE, DWORD, ULONGLONG, PWCHAR, PVOID))Utils::ResolveFunction("xam.xex", 656);

	TmpFunc(exnq, dwUserIndex, qwAreas, displayText, contextData);
}