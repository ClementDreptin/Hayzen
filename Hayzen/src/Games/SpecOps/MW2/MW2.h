#pragma once

#include "Core\Game.h"
#include "Elements\HudElem.h"

#include "Games\SpecOps\MW2\Structs.h"


//--------------------------------------------------------------------------------------
// Name: class SpecOpsMW2
// Desc: Class to run Spec Ops MW2, inherits from Game.
//--------------------------------------------------------------------------------------
class SpecOpsMW2 : public Game
{
public:
    virtual VOID Init();
private:
    virtual VOID CreateStructure();

    static VOID Scr_NotifyStub(SpecOpsMW2Structs::gentity_s* entity, USHORT stringValue, UINT paramCount);
    static VOID Scr_NotifyHook(SpecOpsMW2Structs::gentity_s* entity, USHORT stringValue, UINT paramCount);

    static VOID ClientCommandStub(INT clientNum, LPCSTR s);
    static VOID ClientCommandHook(INT clientNum, LPCSTR s);
};
