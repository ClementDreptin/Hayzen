#pragma once

#include "Core\Title.h"
#include "Elements\HudElem.h"

// Class to run Spec Ops MW2, inherits from Game.
class SpecOpsMW2Title : public Title
{
public:
    ~SpecOpsMW2Title();

    // Set the draw function pointers and the function hooks.
    virtual void Init();

private:
    static bool s_HasJumped;

    // Create the structure of the menu and save it a static member.
    virtual void CreateStructure();

    // Pointer to the detour object for ClientCommand.
    static Detour *s_pClientCommandDetour;

    // Initialize the menu when the game starts.
    static void ClientCommandHook(int clientNum, const char *s);
};
