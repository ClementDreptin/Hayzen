#include "pch.h"
#include "Utils\Utils.h"

DWORD Utils::ResolveFunction(const std::string& moduleName, DWORD ordinal)
{
	HMODULE mHandle = GetModuleHandle(moduleName.c_str());

	return (mHandle == NULL) ? NULL : (DWORD)GetProcAddress(mHandle, (LPCSTR)ordinal);
}