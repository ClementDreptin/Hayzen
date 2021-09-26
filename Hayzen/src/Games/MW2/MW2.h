#pragma once

#include "Core\Game.h"
#include "Elements\HudElem.h"

#include "Games\MW2\Structs.h"


//--------------------------------------------------------------------------------------
// Name: class MW2
// Desc: Class to run MW2 TU9, inherits from Game.
//--------------------------------------------------------------------------------------
class MW2 : public Game
{
public:
    virtual VOID Init();
private:
    virtual VOID CreateStructure();

    static VOID Scr_NotifyStub(MW2Structs::gentity_s* entity, USHORT stringValue, UINT paramCount);
    static VOID Scr_NotifyHook(MW2Structs::gentity_s* entity, USHORT stringValue, UINT paramCount);

    static VOID SV_ExecuteClientCommandStub(INT client, LPCSTR s, INT clientOK, INT fromOldServer);
    static VOID SV_ExecuteClientCommandHook(INT client, LPCSTR s, INT clientOK, INT fromOldServer);
};
