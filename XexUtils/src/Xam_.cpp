#include "pch.h"
#include "Xam_.h"

#include "Memory.h"
#include "Formatter.h"

namespace XexUtils
{
namespace Xam
{
	void XNotifyQueueUI(XNOTIFYQUEUEUI_TYPE exnq, DWORD dwUserIndex, ULONGLONG qwAreas, PWCHAR displayText, PVOID contextData)
	{
		void(__cdecl* TmpFunc)(
			XNOTIFYQUEUEUI_TYPE exnq,
			DWORD dwUserIndex,
			ULONGLONG qwAreas,
			PWCHAR displayText,
			PVOID contextData
		) = (void(__cdecl*)(XNOTIFYQUEUEUI_TYPE, DWORD, ULONGLONG, PWCHAR, PVOID))Memory::ResolveFunction("xam.xex", 656);

		TmpFunc(exnq, dwUserIndex, qwAreas, displayText, contextData);
	}

	void XNotify(const std::string& text, XNOTIFYQUEUEUI_TYPE type)
	{
		XNotifyQueueUI(type, 0xFF, XNOTIFY_SYSTEM, (PWCHAR)(Formatter::ToWide(text).c_str()), 0);
	}
}
}