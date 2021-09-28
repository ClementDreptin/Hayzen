#pragma once

#include "Core\Game.h"
#include "Elements\HudElem.h"

#include "Games\SpecOps\MW3\Structs.h"


//--------------------------------------------------------------------------------------
// Name: class SpecOpsMW3
// Desc: Class to run Spec Ops MW3, inherits from Game.
//--------------------------------------------------------------------------------------
class SpecOpsMW3 : public Game
{
public:
    virtual VOID Init();
private:
    virtual VOID CreateStructure();

    static VOID ClientCommandStub(INT clientNum, LPCSTR s);
    static VOID ClientCommandHook(INT clientNum, LPCSTR s);

    static VOID PlayerCmd_AllowJumpStub();
    static VOID PlayerCmd_AllowJumpHook();
};
