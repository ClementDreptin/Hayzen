#include "pch.h"
#include "Games\SpecOps\MW3\MenuFunctions.h"

using namespace SpecOpsMW3GameFunctions;


//--------------------------------------------------------------------------------------
// Name: ToggleGodMode()
// Desc: Toggle God Mode.
//--------------------------------------------------------------------------------------
VOID SpecOpsMW3MenuFunctions::ToggleGodMode(Menu* pMenu)
{
    INT iClientNum = pMenu->GetClientNum();
    playerState_s* playerState = SV_GetPlayerstateForClientNum(iClientNum);

    if (playerState->otherFlags == 0)
    {
        playerState->otherFlags = 1;
        pMenu->SetFeedbackText("God Mode ^2On");
    }
    else
    {
        playerState->otherFlags = 0;
        pMenu->SetFeedbackText("God Mode ^1Off");
    }
}


//--------------------------------------------------------------------------------------
// Name: ToggleAmmo()
// Desc: Toggle unlimited ammo.
//--------------------------------------------------------------------------------------
VOID SpecOpsMW3MenuFunctions::ToggleAmmo(Menu* pMenu)
{
    DWORD dwAddress = 0x8235BB54;
    DWORD dwDefaultValue = 0x7D3D5050;
    DWORD dwModifiedValue = 0x7D495378;

    if (Memory::Read<DWORD>(dwAddress) == dwDefaultValue)
    {
        Memory::Write<DWORD>(dwAddress, dwModifiedValue);
        pMenu->SetFeedbackText("Unlimited Ammo ^2On");
    }
    else
    {
        Memory::Write<DWORD>(dwAddress, dwDefaultValue);
        pMenu->SetFeedbackText("Unlimited Ammo ^1Off");
    }
}


//--------------------------------------------------------------------------------------
// Name: ToggleSaveLoadBinds()
// Desc: Toggle save and load binds.
//--------------------------------------------------------------------------------------
VOID SpecOpsMW3MenuFunctions::ToggleSaveLoadBinds(Menu* pMenu)
{
    if (!pMenu->BindsEnabled())
        pMenu->SetFeedbackText("Press " CHAR_RB " to ^2Save^7 and " CHAR_LB " to ^2Load");
    else
        pMenu->SetFeedbackText("Save and Load binds ^1Off");

    pMenu->ToggleBinds();
}


//--------------------------------------------------------------------------------------
// Name: SavePosition()
// Desc: Save the current player's position.
//--------------------------------------------------------------------------------------
VOID SpecOpsMW3MenuFunctions::SavePosition(Menu* pMenu)
{
    INT iClientNum = pMenu->GetClientNum();

    pMenu->SetSavedPos(SV_GetPlayerstateForClientNum(iClientNum)->origin);
    pMenu->SetSavedAngles(SV_GetPlayerstateForClientNum(iClientNum)->viewAngles);

    pMenu->SetFeedbackText("Position ^2Saved");
}


//--------------------------------------------------------------------------------------
// Name: LoadPosition()
// Desc: Load the previously saved player's position.
//--------------------------------------------------------------------------------------
VOID SpecOpsMW3MenuFunctions::LoadPosition(Menu* pMenu)
{
    INT iClientNum = pMenu->GetClientNum();
    CONST vec3& SavedPos = pMenu->GetSavedPos();
    CONST vec3& SavedAngles = pMenu->GetSavedAngles();

    // Make sure the player previously saved their position
    if (SavedPos == vec3(0.0f, 0.0f, 0.0f) || SavedAngles == vec3(0.0f, 0.0f, 0.0f))
    {
        pMenu->SetFeedbackText("^1Save a position first!");
        return;
    }

    TeleportPlayer(GetEntity(iClientNum), (PFLOAT)&SavedPos, (PFLOAT)&SavedAngles);
}


//--------------------------------------------------------------------------------------
// Name: ToggleUFO()
// Desc: Toggle UFO.
//--------------------------------------------------------------------------------------
VOID SpecOpsMW3MenuFunctions::ToggleUFO(Menu* pMenu)
{
    INT iClientNum = pMenu->GetClientNum();

    if (GetGClient(iClientNum)->mFlags != 2)
    {
        GetGClient(iClientNum)->mFlags = 2;
        pMenu->SetFeedbackText("Ufo ^2On");
    }
    else
    {
        GetGClient(iClientNum)->mFlags = 0;
        pMenu->SetFeedbackText("Ufo ^1Off");
    }
}
