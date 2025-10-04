#include "pch.h"
#include "Modules/NotificationPatcher.h"

namespace NotificationPatcher
{

uint16_t s_DefaultInstruction = 0;
const uintptr_t s_RetailPatchAddress = 0x816A3158;
const uintptr_t s_DevkitPatchAddress = 0x817619DC;

void Enable()
{
    // Save the default instruction
    if (s_DefaultInstruction == 0)
        s_DefaultInstruction = Memory::Read<uint16_t>(
            IsDevkit() ? s_DevkitPatchAddress : s_RetailPatchAddress
        );

    // Turns "bne cr6, loc_816A3174" into "b loc_816A3174" on retail, and
    // "bne cr6, loc_81761A04" into "b loc_81761A04" on devkit, which basically bypasses a
    // "if (KeGetCurrentProcessType() != PROC_SYSTEM)" in XMsgProcessRequest
    Memory::Write<uint16_t>(IsDevkit() ? s_DevkitPatchAddress : s_RetailPatchAddress, 0x4800);
}

void Disable()
{
    // Do nothing if notifications were never patched
    if (s_DefaultInstruction == 0)
        return;

    Memory::Write<uint16_t>(
        IsDevkit() ? s_DevkitPatchAddress : s_RetailPatchAddress,
        s_DefaultInstruction
    );
}

}
