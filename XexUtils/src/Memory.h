#pragma once

namespace XexUtils
{
namespace Memory
{
	DWORD ResolveFunction(const std::string& moduleName, DWORD ordinal);

	void Thread(LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameters = 0);

	void PatchInJump(DWORD* address, DWORD destination, BOOL linked);

	void HookFunctionStart(DWORD* address, DWORD* saveStub, DWORD destination);

	DWORD RelinkGPLR(int offset, DWORD* saveStubAddr, DWORD* orgAddr);

	template<typename T>
	void Write(DWORD address, T data)
	{
		if (!MmIsAddressValid((DWORD*)address))
		{
			DbgPrint("Invalid address: %#010x\n", address);
			return;
		}

		*(T*)address = data;
	}

	template<typename T>
	T Read(DWORD address)
	{
		if (!MmIsAddressValid((DWORD*)address))
		{
			DbgPrint("Invalid address: %#010x\n", address);
			return 0;
		}

		return *(T*)address;
	}
}
}