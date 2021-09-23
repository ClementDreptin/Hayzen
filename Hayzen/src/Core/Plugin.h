#pragma once

#include "Core\Game.h"


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
    static Game* s_CurrentGame;

    static DWORD Update(LPVOID);
    static VOID InitNewGame(DWORD dwNewTitle);
};
