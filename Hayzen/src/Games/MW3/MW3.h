#pragma once

#include "Core\Game.h"
#include "Elements\HudElem.h"

#include "Games\MW3\Structs.h"


//--------------------------------------------------------------------------------------
// Name: class MW3
// Desc: Class to run MW3 TU24, inherits from Game.
//--------------------------------------------------------------------------------------
class MW3 : public Game
{
public:
    virtual VOID Init();
private:
    virtual VOID CreateStructure();

    static VOID Scr_NotifyStub(MW3Structs::gentity_s* entity, USHORT stringValue, UINT paramCount);
    static VOID Scr_NotifyHook(MW3Structs::gentity_s* entity, USHORT stringValue, UINT paramCount);

    static VOID SV_ExecuteClientCommandStub(INT client, LPCSTR s, INT clientOK, INT fromOldServer);
    static VOID SV_ExecuteClientCommandHook(INT client, LPCSTR s, INT clientOK, INT fromOldServer);
};
