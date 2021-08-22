#pragma once

namespace Plugin
{
    extern BOOL Running;
    extern DWORD CurrentTitle;

    enum Games : DWORD
    {
        DASHBOARD = 0xFFFE07D1,
        MW2 = 0x41560817,
        MW3 = 0x415608CB
    };

    VOID Start();
    VOID Stop();

    DWORD MonitorTitleId(LPVOID lpThreadParameter);
    VOID InitNewGame(DWORD titleId);

    // Makes sure games get reset properly
    VOID Cleanup();
}