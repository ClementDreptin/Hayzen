#pragma once

// Notifications don't work when created from system threads, this module patches XAM to allow it.
//
// The same patch is usually applied by RPC servers (JRPC2, XDRPC, etc.) and most RGH/Jtag users have one loaded,
// but most Devkit and BadUpdate users don't, so this module makes sure notifications work for everyone.

class NotificationPatcher
{
public:
    NotificationPatcher();

    ~NotificationPatcher();

private:
    uint16_t m_DefaultInstruction;

    static const uintptr_t s_RetailPatchAddress = 0x816A3158;
    static const uintptr_t s_DevkitPatchAddress = 0x817619DC;
};
