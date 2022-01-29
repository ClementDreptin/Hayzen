#include "pch.h"
#include "Games\SpecOps\MW3\MenuFunctions.h"

using namespace SpecOpsMW3GameFunctions;


void SpecOpsMW3MenuFunctions::ToggleGodMode(Menu *pMenu)
{
    int iClientNum = pMenu->GetClientNum();

    playerState_s *playerState = SV_GetPlayerstateForClientNum(iClientNum);

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

void SpecOpsMW3MenuFunctions::ToggleAmmo(Menu *pMenu)
{
    int iClientNum = pMenu->GetClientNum();

    DWORD dwAddress = 0x8235BB54;
    DWORD dwDefaultValue = 0x7D3D5050;
    DWORD dwModifiedValue = 0x7D495378;

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

void SpecOpsMW3MenuFunctions::ToggleSaveLoadBinds(Menu *pMenu)
{
    int iClientNum = pMenu->GetClientNum();

    if (!pMenu->BindsEnabled())
    {
        Cbuf_AddText(0, "unbind button_lshldr;unbind button_rshldr");
        iPrintLn(iClientNum, "Press " CHAR_RB " to ^2Save^7 and " CHAR_LB " to ^2Load");
    }
    else
    {
        Cbuf_AddText(0, "bind button_lshldr \"+smoke\";bind button_rshldr \"+frag\"");
        iPrintLn(iClientNum, "Save and Load binds ^1Off");
    }

    pMenu->ToggleBinds();
}

void SpecOpsMW3MenuFunctions::SavePosition(Menu *pMenu)
{
    int iClientNum = pMenu->GetClientNum();

    pMenu->SetSavedPos(SV_GetPlayerstateForClientNum(iClientNum)->origin);
    pMenu->SetSavedAngles(SV_GetPlayerstateForClientNum(iClientNum)->viewAngles);

    iPrintLn(iClientNum, "Position ^2Saved");
}

void SpecOpsMW3MenuFunctions::LoadPosition(Menu *pMenu)
{
    int iClientNum = pMenu->GetClientNum();

    // Make sure the player previously saved their position
    if (!pMenu->HasSavedPosition())
    {
        iPrintLn(iClientNum, "^1Save a position first!");
        return;
    }

    // Request tht menu for a teleport
    pMenu->RequestTeleport();
}

void SpecOpsMW3MenuFunctions::ToggleUFO(Menu *pMenu)
{
    int iClientNum = pMenu->GetClientNum();

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

void SpecOpsMW3MenuFunctions::ToggleSecondPlayerGodMode(Menu *pMenu)
{
    // The second client num is always 1
    int iSecondClientNum = 1;
    int iFirstClientNum = pMenu->GetClientNum();

    // If the player name of the second client is empty, it means there is no second client
    gclient_s *pSecondClient = GetGClient(iSecondClientNum);
    if (!pSecondClient->connected)
    {
        iPrintLn(iFirstClientNum, "^1No other player in the game!");
        return;
    }

    playerState_s *playerState = SV_GetPlayerstateForClientNum(iSecondClientNum);

    if (playerState->otherFlags == 0)
    {
        playerState->otherFlags = 1;
        iPrintLn(iFirstClientNum, "Second Player God Mode ^2On");
    }
    else
    {
        playerState->otherFlags = 0;
        iPrintLn(iFirstClientNum, "Second Player God Mode ^1Off");
    }
}

void SpecOpsMW3MenuFunctions::TeleportSecondPlayerToMe(Menu *pMenu)
{
    // The second client num is always 1
    int iSecondClientNum = 1;
    int iFirstClientNum = pMenu->GetClientNum();

    gclient_s *pSecondClient = GetGClient(iSecondClientNum);
    if (!pSecondClient->connected)
    {
        iPrintLn(iFirstClientNum, "^1No other player in the game!");
        return;
    }

    // Get the first player's current position
    float fDistance = 100.0f;
    vec3 Origin = SV_GetPlayerstateForClientNum(iFirstClientNum)->origin;
    float fViewY = SV_GetPlayerstateForClientNum(iFirstClientNum)->viewAngles.y;

    // Teleport the second player in front of the first player
    pSecondClient->ps.origin = Math::ToFront(Origin, fViewY, fDistance);
}
