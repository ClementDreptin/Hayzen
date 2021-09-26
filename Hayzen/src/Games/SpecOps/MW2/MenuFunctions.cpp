#include "pch.h"
#include "Games\SpecOps\MW2\MenuFunctions.h"

using namespace SpecOpsMW2GameFunctions;


//--------------------------------------------------------------------------------------
// Name: ToggleGodMode()
// Desc: Toggle God Mode.
//--------------------------------------------------------------------------------------
VOID SpecOpsMW2MenuFunctions::ToggleGodMode(Menu* pMenu)
{
    INT iClientNum = pMenu->GetClientNum();
    playerState_s* playerState = SV_GetPlayerstateForClientNum(iClientNum);

    if (playerState->otherFlags == 0)
    {
        playerState->otherFlags = 1;
        iPrintLn(iClientNum, "God Mode ^2On");
    }
    else
    {
        playerState->otherFlags = 0;
        iPrintLn(iClientNum, "God Mode ^1Off");
    }
}


//--------------------------------------------------------------------------------------
// Name: ToggleAmmo()
// Desc: Toggle unlimited ammo.
//--------------------------------------------------------------------------------------
VOID SpecOpsMW2MenuFunctions::ToggleAmmo(Menu* pMenu)
{
    INT iClientNum = pMenu->GetClientNum();
    DWORD dwAddress = 0x82331F48;
    DWORD dwDefaultValue = 0x7D1D4850;
    DWORD dwModifiedValue = 0x7D284B78;

    if (Memory::Read<DWORD>(dwAddress) == dwDefaultValue)
    {
        Memory::Write<DWORD>(dwAddress, dwModifiedValue);
        iPrintLn(iClientNum, "Unlimited Ammo ^2On");
    }
    else
    {
        Memory::Write<DWORD>(dwAddress, dwDefaultValue);
        iPrintLn(iClientNum, "Unlimited Ammo ^1Off");
    }
}


//--------------------------------------------------------------------------------------
// Name: ToggleSaveLoadBinds()
// Desc: Toggle save and load binds.
//--------------------------------------------------------------------------------------
VOID SpecOpsMW2MenuFunctions::ToggleSaveLoadBinds(Menu* pMenu)
{
    INT iClientNum = pMenu->GetClientNum();

    if (!pMenu->BindsEnabled())
        iPrintLn(iClientNum, "Press RB to ^2Save^7 and LB to ^2Load");
    else
        iPrintLn(iClientNum, "Save and Load binds ^1Off");

    pMenu->ToggleBinds();
}


//--------------------------------------------------------------------------------------
// Name: SavePosition()
// Desc: Save the current player's position.
//--------------------------------------------------------------------------------------
VOID SpecOpsMW2MenuFunctions::SavePosition(Menu* pMenu)
{
    INT iClientNum = pMenu->GetClientNum();

    pMenu->SetSavedPos(SV_GetPlayerstateForClientNum(iClientNum)->origin);
    pMenu->SetSavedAngles(SV_GetPlayerstateForClientNum(iClientNum)->viewAngles);

    iPrintLn(iClientNum, "Position ^2Saved");
}


//--------------------------------------------------------------------------------------
// Name: LoadPosition()
// Desc: Load the previously saved player's position.
//--------------------------------------------------------------------------------------
VOID SpecOpsMW2MenuFunctions::LoadPosition(Menu* pMenu)
{
    INT iClientNum = pMenu->GetClientNum();
    CONST vec3& SavedPos = pMenu->GetSavedPos();
    CONST vec3& SavedAngles = pMenu->GetSavedAngles();

    // Make sure the player previously saved their position
    if (SavedPos == vec3(0.0f, 0.0f, 0.0f) || SavedAngles == vec3(0.0f, 0.0f, 0.0f))
    {
        iPrintLn(iClientNum, "^1Save a position first!");
        return;
    }

    TeleportPlayer(GetEntity(iClientNum), (PFLOAT)&SavedPos, (PFLOAT)&SavedAngles);
}


//--------------------------------------------------------------------------------------
// Name: ToggleUFO()
// Desc: Toggle UFO.
//--------------------------------------------------------------------------------------
VOID SpecOpsMW2MenuFunctions::ToggleUFO(Menu* pMenu)
{
    INT iClientNum = pMenu->GetClientNum();

    if (GetGClient(iClientNum)->mFlags != 2)
    {
        GetGClient(iClientNum)->mFlags = 2;
        iPrintLn(iClientNum, "Ufo ^2On");
    }
    else
    {
        GetGClient(iClientNum)->mFlags = 0;
        iPrintLn(iClientNum, "Ufo ^1Off");
    }
}
