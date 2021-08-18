#pragma once

namespace Plugin
{
    extern bool Running;
    extern DWORD CurrentTitle;

    enum Games : DWORD
    {
        DASHBOARD = 0xFFFE07D1,
        MW2 = 0x41560817,
        MW3 = 0x415608CB
    };

    void Start();
    void Stop();

    DWORD MonitorTitleId(LPVOID lpThreadParameter);
    void InitNewGame(DWORD titleId);

    // Makes sure games get reset properly
    void Cleanup();
}