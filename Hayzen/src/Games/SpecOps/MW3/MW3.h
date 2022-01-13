#pragma once

#include "Core\Game.h"
#include "Elements\HudElem.h"

#include "Games\SpecOps\MW3\Structs.h"


// Class to run Spec Ops MW3, inherits from Game.
class SpecOpsMW3 : public Game
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

    // Stub to hold the original code of PlayerCmd_AllowJump.
    static void PlayerCmd_AllowJumpStub();

    // Make the PlayerCmd_AllowJump function not do anything to be able to jump in missions where you normally can't.
    static void PlayerCmd_AllowJumpHook();
};
