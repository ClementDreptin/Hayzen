#pragma once

#include "Games\Alpha\MW2\Client.h"
#include "Games\Alpha\MW2\Structs.h"

namespace Alpha
{
namespace MW2
{
    // Variables
    extern BOOL HasGameBegun;
    extern std::unordered_map<INT, Client> Clients;

    // Functions
    VOID Init();
    VOID SetupGame(INT clientNum);
    VOID ResetGame(INT clientNum, BOOL resetBot = TRUE);
    BOOL Verify(INT clientNum);
    VOID SafeReset(); // Resets everything when the game was not reset properly

    // Hooks and their stubs
    VOID SCR_DrawScreenFieldStub(CONST INT localClientNum, INT refreshedUI);
    VOID SCR_DrawScreenFieldHook(CONST INT localClientNum, INT refreshedUI);

    VOID Scr_NotifyStub(gentity_s* entity, USHORT stringValue, UINT paramCount);
    VOID Scr_NotifyHook(gentity_s* entity, USHORT stringValue, UINT paramCount);

    VOID SV_ExecuteClientCommandStub(INT client, LPCSTR s, INT clientOK, INT fromOldServer);
    VOID SV_ExecuteClientCommandHook(INT client, LPCSTR s, INT clientOK, INT fromOldServer);
}
}