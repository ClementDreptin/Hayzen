#pragma once


//--------------------------------------------------------------------------------------
// Name: class Plugin
// Desc: Main class that serves as entry point.
//--------------------------------------------------------------------------------------
class Plugin
{
public:
    static VOID Start();
    static VOID Stop();
private:
    static BOOL s_bRunning;
    static DWORD s_dwCurrentTitle;

    static DWORD MonitorTitleId(LPVOID);
    static VOID InitNewGame(DWORD dwNewTitle);
    static VOID Cleanup();
};
