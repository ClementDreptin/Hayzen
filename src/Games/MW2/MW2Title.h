#pragma once

#include "Core\Title.h"
#include "Elements\HudElem.h"

#include "Games\MW2\Structs.h"

// Class to run MW2 TU9, inherits from Title.
class MW2Title : public Title
{
public:
    ~MW2Title();

    // Set the draw function pointers and the function hooks.
    virtual void Init();

private:
    // Create the structure of the menu and save it a static member.
    virtual void CreateStructure();

    // Pointer to the detour object for Scr_Notify.
    static Detour *s_pScr_NotifyDetour;

    // Initialize the menu when the game starts.
    static void Scr_NotifyHook(MW2::Game::gentity_s *entity, uint16_t stringValue, uint32_t paramCount);

    // Pointer to the detour object for SV_ExecuteClientCommand.
    static Detour *s_pSV_ExecuteClientCommandDetour;

    // Stop the menu when the game ends.
    static void SV_ExecuteClientCommandHook(int client, const char *s, int clientOK, int fromOldServer);
};
