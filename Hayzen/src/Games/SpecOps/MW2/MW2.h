#pragma once

#include "Core\Game.h"
#include "Elements\HudElem.h"

#include "Games\SpecOps\MW2\Structs.h"


// Class to run Spec Ops MW2, inherits from Game.
class SpecOpsMW2 : public Game
{
public:
    // Set the draw function pointers and the function hooks.
    virtual VOID Init();
private:
    static BOOL s_bJumped;

    // Create the structure of the menu and save it a static member.
    virtual VOID CreateStructure();

    // Stub to hold the original code of ClientCommand.
    static VOID ClientCommandStub(INT clientNum, LPCSTR s);

    // Initialize the menu when the game starts.
    static VOID ClientCommandHook(INT clientNum, LPCSTR s);
};
