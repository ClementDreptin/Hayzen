#pragma once

#include "Games\SpecOps\MW3\Client.h"

namespace SpecOps
{
namespace MW3
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
    VOID DrawMenuDebugTextStub(INT dc);
    VOID DrawMenuDebugTextHook(INT dc);

    VOID ClientCommandStub(INT clientNum, LPCSTR s);
    VOID ClientCommandHook(INT clientNum, LPCSTR s);

    VOID PlayerCmd_AllowJumpStub();
    VOID PlayerCmd_AllowJumpHook();
}
}