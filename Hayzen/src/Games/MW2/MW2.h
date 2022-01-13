#pragma once

#include "Core\Game.h"
#include "Elements\HudElem.h"

#include "Games\MW2\Structs.h"


// Class to run MW2 TU9, inherits from Game.
class MW2 : public Game
{
public:
    // Set the draw function pointers and the function hooks.
    virtual void Init();
private:
    // Create the structure of the menu and save it a static member.
    virtual void CreateStructure();

    // Stub to hold the original code of Scr_Notify.
    static void Scr_NotifyStub(MW2Structs::gentity_s *entity, uint16_t stringValue, uint32_t paramCount);

    // Initialize the menu when the game starts.
    static void Scr_NotifyHook(MW2Structs::gentity_s *entity, uint16_t stringValue, uint32_t paramCount);

    // Stub to hold the original code of SV_ExecuteClientCommand.
    static void SV_ExecuteClientCommandStub(int client, const char *s, int clientOK, int fromOldServer);

    // Stop the menu when the game ends.
    static void SV_ExecuteClientCommandHook(int client, const char *s, int clientOK, int fromOldServer);
};
