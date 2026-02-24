#include "pch.h"
#include "Modules/NotificationPatcher.h"

NotificationPatcher::NotificationPatcher()
    : m_DefaultInstruction(Memory::Read<uint16_t>(IsDevkit() ? s_DevkitPatchAddress : s_RetailPatchAddress))
{
    // Turns "bne cr6, loc_816A3174" into "b loc_816A3174" on retail, and
    // "bne cr6, loc_81761A04" into "b loc_81761A04" on devkit, which basically bypasses a
    // "if (KeGetCurrentProcessType() != PROC_TYPE_SYSTEM)" in XMsgProcessRequest
    Memory::Write<uint16_t>(IsDevkit() ? s_DevkitPatchAddress : s_RetailPatchAddress, 0x4800);
}

NotificationPatcher::~NotificationPatcher()
{
    Memory::Write<uint16_t>(
        IsDevkit() ? s_DevkitPatchAddress : s_RetailPatchAddress,
        m_DefaultInstruction
    );
}
