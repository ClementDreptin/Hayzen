#pragma once

namespace Utils
{
	DWORD ResolveFunction(const std::string& moduleName, DWORD ordinal);

	void Thread(LPTHREAD_START_ROUTINE lpStartAddress);

	void PatchInJump(DWORD* address, DWORD destination, BOOL linked);

	void HookFunctionStart(DWORD* address, DWORD* saveStub, DWORD destination);

	DWORD RelinkGPLR(int offset, DWORD* saveStubAddr, DWORD* orgAddr);

	void XNotify(const std::string& text, XNOTIFYQUEUEUI_TYPE type = XNOTIFYUI_TYPE_PREFERRED_REVIEW);
}