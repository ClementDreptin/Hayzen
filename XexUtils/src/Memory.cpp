#include "pch.h"
#include "Memory.h"

namespace XexUtils
{
namespace Memory
{
	DWORD ResolveFunction(const std::string& moduleName, DWORD ordinal)
	{
		HMODULE mHandle = GetModuleHandle(moduleName.c_str());

		return (mHandle == NULL) ? NULL : (DWORD)GetProcAddress(mHandle, (LPCSTR)ordinal);
	}

	void Thread(LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameters)
	{
		HANDLE hThread;
		DWORD dwThreadId;
		ExCreateThread(&hThread, 0, &dwThreadId, (PVOID)XapiThreadStartup , lpStartAddress, lpParameters, 2);
		ResumeThread(hThread);
		CloseHandle(hThread);
	}

	void PatchInJump(DWORD* address, DWORD destination, BOOL linked)
	{
		DWORD writeBuffer;

		if(destination & 0x8000)
			writeBuffer = 0x3D600000 + (((destination >> 16) & 0xFFFF) + 1);
		else
			writeBuffer = 0x3D600000 + ((destination >> 16) & 0xFFFF);

		address[0] = writeBuffer;
		writeBuffer = 0x396B0000 + (destination & 0xFFFF);
		address[1] = writeBuffer;
		writeBuffer = 0x7D6903A6;
		address[2] = writeBuffer;

		if (linked)
			writeBuffer = 0x4E800421;
		else
			writeBuffer = 0x4E800420;

		address[3] = writeBuffer;

		__dcbst(0, address);
		__sync();
		__isync();
	}

	void HookFunctionStart(DWORD* address, DWORD* saveStub, DWORD destination)
	{
		if (saveStub != NULL && address != NULL)
		{
			int i;
			DWORD addrReloc = (DWORD)(&address[4]);
			DWORD writeBuffer;

			if(addrReloc & 0x8000)
				writeBuffer = 0x3D600000 + (((addrReloc >> 16) & 0xFFFF) + 1);
			else
				writeBuffer = 0x3D600000 + ((addrReloc >> 16) & 0xFFFF);

			saveStub[0] = writeBuffer;
			writeBuffer = 0x396B0000 + (addrReloc & 0xFFFF);
			saveStub[1] = writeBuffer;
			writeBuffer = 0x7D6903A6;
			saveStub[2] = writeBuffer;
		
			for (i = 0; i < 4; i++)
			{
				if ((address[i] & 0x48000003) == 0x48000001)
				{
					writeBuffer = RelinkGPLR((address[i] &~ 0x48000003), &saveStub[i + 3], &address[i]);
					saveStub[i + 3] = writeBuffer;
				}
				else
				{
					writeBuffer = address[i];
					saveStub[i + 3] = writeBuffer;
				}
			}

			writeBuffer = 0x4E800420; // bctr
			saveStub[7] = writeBuffer;

			__dcbst(0, saveStub);
			__sync();
			__isync();

			PatchInJump(address, destination, FALSE);
		}
	}

	VOID __declspec(naked) GLPR_FUN(VOID)
	{
		__asm
		{
			std     r14, -0x98(sp)
			std     r15, -0x90(sp)
			std     r16, -0x88(sp)
			std     r17, -0x80(sp)
			std     r18, -0x78(sp)
			std     r19, -0x70(sp)
			std     r20, -0x68(sp)
			std     r21, -0x60(sp)
			std     r22, -0x58(sp)
			std     r23, -0x50(sp)
			std     r24, -0x48(sp)
			std     r25, -0x40(sp)
			std     r26, -0x38(sp)
			std     r27, -0x30(sp)
			std     r28, -0x28(sp)
			std     r29, -0x20(sp)
			std     r30, -0x18(sp)
			std     r31, -0x10(sp)
			stw     r12, -0x8(sp)
			blr
		}
	}

	DWORD RelinkGPLR(int offset, DWORD* saveStubAddr, DWORD* orgAddr)
	{
		DWORD inst = 0, repl;
		int i;
		PDWORD saver = (PDWORD)GLPR_FUN;

		if (offset & 0x2000000)
			offset = offset|0xFC000000;

		repl = orgAddr[offset / 4];

		for (i = 0; i < 20; i++)
		{
			if (repl == saver[i])
			{
				int newOffset = (int)&saver[i]-(int)saveStubAddr;
				inst = 0x48000001|(newOffset&0x3FFFFFC);
			}
		}

		return inst;
	}
}
}