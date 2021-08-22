#pragma once

#include "Games\SpecOps\MW2\Client.h"

namespace SpecOps
{
namespace MW2
{
    // Variables
    extern BOOL HasGameBegun;
    extern std::unordered_map<INT, Client> Clients;

    // Functions
    VOID Init();
    VOID SetupGame(INT clientNum);
    BOOL Verify(INT clientNum);
    VOID SafeReset(); // Resets everything when the game was not reset properly

    // Hooks and their stubs
    VOID SCR_DrawScreenFieldStub(INT refreshedUI);
    VOID SCR_DrawScreenFieldHook(INT refreshedUI);

    VOID ClientCommandStub(INT clientNum, LPCSTR s);
    VOID ClientCommandHook(INT clientNum, LPCSTR s);
}
}