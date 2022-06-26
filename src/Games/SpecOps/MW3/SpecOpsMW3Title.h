#pragma once

#include "Core\Title.h"
#include "Elements\HudElem.h"

// Class to run Spec Ops MW3, inherits from Game.
class SpecOpsMW3Title : public Title
{
public:
    ~SpecOpsMW3Title();

    // Set the draw function pointers and the function hooks.
    virtual void Init();

private:
    static bool s_bJumped;

    // Create the structure of the menu and save it a static member.
    virtual void CreateStructure();

    // Pointer to the detour object for ClientCommand.
    static Detour *s_pClientCommandDetour;

    // Initialize the menu when the game starts.
    static void ClientCommandHook(int clientNum, const char *s);

    // Pointer to the detour object for PlayerCmd_AllowJump.
    static Detour *s_pPlayerCmd_AllowJumpDetour;

    // Make the PlayerCmd_AllowJump function not do anything to be able to jump in missions where you normally can't.
    static void PlayerCmd_AllowJumpHook();
};
