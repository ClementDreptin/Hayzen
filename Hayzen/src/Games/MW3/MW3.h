#pragma once

#include "Core\Game.h"
#include "Elements\HudElem.h"

#include "Games\MW3\Structs.h"


// Class to run MW3 TU24, inherits from Game.
class MW3 : public Game
{
public:
    // Set the draw function pointers and the function hooks.
    virtual VOID Init();
private:
    // Create the structure of the menu and save it a static member.
    virtual VOID CreateStructure();

    // Stub to hold the original code of Scr_Notify.
    static VOID Scr_NotifyStub(MW3Structs::gentity_s *entity, USHORT stringValue, UINT paramCount);

    // Initialize the menu when the game starts.
    static VOID Scr_NotifyHook(MW3Structs::gentity_s *entity, USHORT stringValue, UINT paramCount);

    // Stub to hold the original code of SV_ExecuteClientCommand.
    static VOID SV_ExecuteClientCommandStub(INT client, LPCSTR s, INT clientOK, INT fromOldServer);

    // Stop the menu when the game ends.
    static VOID SV_ExecuteClientCommandHook(INT client, LPCSTR s, INT clientOK, INT fromOldServer);
};
