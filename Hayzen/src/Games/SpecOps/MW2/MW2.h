#pragma once

#include "Core\Game.h"
#include "Elements\HudElem.h"


// Class to run Spec Ops MW2, inherits from Game.
class SpecOpsMW2 : public Game
{
public:
    // Set the draw function pointers and the function hooks.
    virtual void Init();
private:
    static bool s_bJumped;

    // Create the structure of the menu and save it a static member.
    virtual void CreateStructure();

    // Stub to hold the original code of ClientCommand.
    static void ClientCommandStub(int clientNum, const char *s);

    // Initialize the menu when the game starts.
    static void ClientCommandHook(int clientNum, const char *s);
};
