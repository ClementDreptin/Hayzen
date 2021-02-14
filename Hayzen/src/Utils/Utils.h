#pragma once

namespace Utils
{
	DWORD ResolveFunction(const std::string& moduleName, DWORD ordinal);

	void Thread(LPTHREAD_START_ROUTINE lpStartAddress);

	void PatchInJump(DWORD* address, DWORD destination, BOOL linked);

	void XNotify(const std::string& text, XNOTIFYQUEUEUI_TYPE type = XNOTIFYUI_TYPE_PREFERRED_REVIEW);
}