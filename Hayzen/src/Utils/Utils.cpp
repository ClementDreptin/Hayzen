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
    DWORD writeBuffer;
#ifdef _DEBUG
	DWORD outInt;
#endif
	if(destination & 0x8000) // If bit 16 is 1
		writeBuffer = 0x3D600000 + (((destination >> 16) & 0xFFFF) + 1); // lis %r11, dest>>16 + 1
	else
		writeBuffer = 0x3D600000 + ((destination >> 16) & 0xFFFF); // lis %r11, dest>>16
#ifdef _DEBUG
	DmSetMemory(&addr[0], 4, &writeBuffer, &outInt);
#elif defined(NDEBUG)
	address[0] = writeBuffer;
#endif

	writeBuffer = 0x396B0000 + (destination & 0xFFFF); // addi %r11, %r11, dest&0xFFFF
#ifdef _DEBUG
	DmSetMemory(&addr[1], 4, &writeBuffer, &outInt);
#elif defined(NDEBUG)
	address[1] = writeBuffer;
#endif
	writeBuffer = 0x7D6903A6; // mtctr %r11
#ifdef _DEBUG
	DmSetMemory(&addr[2], 4, &writeBuffer, &outInt);
#elif defined(NDEBUG)
	address[2] = writeBuffer;
#endif

	if(linked)
		writeBuffer = 0x4E800421; // bctrl
	else
		writeBuffer = 0x4E800420; // bctr
#ifdef _DEBUG
	DmSetMemory(&addr[3], 4, &writeBuffer, &outInt);
#elif defined(NDEBUG)
	address[3] = writeBuffer;
#endif
	__dcbst(0, address);
	__sync();
	__isync();
}

void Utils::HookFunctionStart(DWORD* address, DWORD* saveStub, DWORD destination)
{
	if((saveStub != NULL)&&(address != NULL))
	{
		int i;
		DWORD addrReloc = (DWORD)(&address[4]);// replacing 4 instructions with a jump, this is the stub return address
		//DbgPrint("hooking addr: %08x savestub: %08x dest: %08x addreloc: %08x\n", addr, saveStub, dest, addrReloc);
		// build the stub
		// make a jump to go to the original function start+4 instructions
		DWORD writeBuffer;
#ifdef _DEBUG
		DWORD outInt;
#endif
		if(addrReloc & 0x8000) // If bit 16 is 1
			writeBuffer = 0x3D600000 + (((addrReloc >> 16) & 0xFFFF) + 1); // lis %r11, dest>>16 + 1
		else
			writeBuffer = 0x3D600000 + ((addrReloc >> 16) & 0xFFFF); // lis %r11, dest>>16
#ifdef _DEBUG
		DmSetMemory(&saveStub[0], 4, &writeBuffer, &outInt);
#endif
#ifdef NDEBUG
		saveStub[0] = writeBuffer;
#endif

		writeBuffer = 0x396B0000 + (addrReloc & 0xFFFF); // addi %r11, %r11, dest&0xFFFF
#ifdef _DEBUG
		DmSetMemory(&saveStub[1], 4, &writeBuffer, &outInt);
#endif
#ifdef NDEBUG
		saveStub[1] = writeBuffer;
#endif
		writeBuffer = 0x7D6903A6; // mtctr %r11
#ifdef _DEBUG
		DmSetMemory(&saveStub[2], 4, &writeBuffer, &outInt);
#endif
#ifdef NDEBUG
		saveStub[2] = writeBuffer;
#endif
		// instructions [3] through [6] are replaced with the original instructions from the function hook
		// copy original instructions over, relink stack frame saves to local ones
		for(i = 0; i<4; i++)
		{
			if((address[i]&0x48000003) == 0x48000001) // branch with link
			{
				//DbgPrint("relink %08x\n", addr[i]);
				writeBuffer = RelinkGPLR((address[i]&~0x48000003), &saveStub[i+3], &address[i]);
#ifdef _DEBUG
				DmSetMemory(&saveStub[i+3], 4, &writeBuffer, &outInt);
#endif
#ifdef NDEBUG
				saveStub[i+3] = writeBuffer;
#endif
			}
			else
			{
				//DbgPrint("copy %08x\n", addr[i]);
				writeBuffer = address[i];
#ifdef _DEBUG
				DmSetMemory(&saveStub[i+3], 4, &writeBuffer, &outInt);
#endif
#ifdef NDEBUG
				saveStub[i+3] = writeBuffer;
#endif
			}
		}
		writeBuffer = 0x4E800420; // bctr
#ifdef _DEBUG
		DmSetMemory(&saveStub[7], 4, &writeBuffer, &outInt);
#endif
#ifdef NDEBUG
		saveStub[7] = writeBuffer;
#endif
		__dcbst(0, saveStub);
		__sync();
		__isync();

		PatchInJump(address, destination, FALSE);
	}
}

VOID __declspec(naked) GLPR_FUN(VOID)
{
	__asm{
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

DWORD Utils::RelinkGPLR(int offset, DWORD* saveStubAddr, DWORD* orgAddr)
{
	DWORD inst = 0, repl;
	int i;
	PDWORD saver = (PDWORD)GLPR_FUN;
	// if the msb is set in the instruction, set the rest of the bits to make the int negative
	if(offset&0x2000000)
		offset = offset|0xFC000000;
	//DbgPrint("frame save offset: %08x\n", offset);
	repl = orgAddr[offset/4];
	//DbgPrint("replacing %08x\n", repl);
	for(i = 0; i < 20; i++)
	{
		if(repl == saver[i])
		{
			int newOffset = (int)&saver[i]-(int)saveStubAddr;
			inst = 0x48000001|(newOffset&0x3FFFFFC);
			//DbgPrint("saver addr: %08x savestubaddr: %08x\n", &saver[i], saveStubAddr);
		}
	}
	//DbgPrint("new instruction: %08x\n", inst);
	return inst;
}

void Utils::XNotify(const std::string& text, XNOTIFYQUEUEUI_TYPE type)
{
	XNotifyQueueUI(type, 0xFF, XNOTIFY_SYSTEM, Formater::CharToWChar(text.c_str()), 0);
}