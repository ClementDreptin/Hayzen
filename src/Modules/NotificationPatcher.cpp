#include "pch.h"
#include "Modules/NotificationPatcher.h"

namespace NotificationPatcher
{

uint32_t s_DefaultInstruction = 0;
const uintptr_t s_PatchAddress = 0x816A3158;

void Enable()
{
    // Save the default instruction
    if (s_DefaultInstruction == 0)
        s_DefaultInstruction = Memory::Read<uint32_t>(s_PatchAddress);

    // Turns "bne cr6, loc_816A3174" into "b loc_816A3174", which basically bypasses a
    // "if (KeGetCurrentProcessType() != PROC_SYSTEM)" in XMsgProcessRequest
    Memory::Write<uint32_t>(s_PatchAddress, 0x4800001C);
}

void Disable()
{
    XASSERT(s_DefaultInstruction != 0);

    Memory::Write<uint32_t>(s_PatchAddress, s_DefaultInstruction);
}

}
