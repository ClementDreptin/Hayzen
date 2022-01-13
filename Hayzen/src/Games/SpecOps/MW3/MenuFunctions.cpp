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
        pMenu->SetFeedbackText("God Mode ^2On");
    }
    else
    {
        playerState->otherFlags = 0;
        pMenu->SetFeedbackText("God Mode ^1Off");
    }
}

void SpecOpsMW3MenuFunctions::ToggleAmmo(Menu *pMenu)
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

void SpecOpsMW3MenuFunctions::ToggleSaveLoadBinds(Menu *pMenu)
{
    if (!pMenu->BindsEnabled())
        pMenu->SetFeedbackText("Press " CHAR_RB " to ^2Save^7 and " CHAR_LB " to ^2Load");
    else
        pMenu->SetFeedbackText("Save and Load binds ^1Off");

    pMenu->ToggleBinds();
}

void SpecOpsMW3MenuFunctions::SavePosition(Menu *pMenu)
{
    int iClientNum = pMenu->GetClientNum();

    pMenu->SetSavedPos(SV_GetPlayerstateForClientNum(iClientNum)->origin);
    pMenu->SetSavedAngles(SV_GetPlayerstateForClientNum(iClientNum)->viewAngles);

    pMenu->SetFeedbackText("Position ^2Saved");
}

void SpecOpsMW3MenuFunctions::LoadPosition(Menu *pMenu)
{
    int iClientNum = pMenu->GetClientNum();
    const vec3 &SavedPos = pMenu->GetSavedPos();
    const vec3 &SavedAngles = pMenu->GetSavedAngles();

    // Make sure the player previously saved their position
    if (SavedPos == vec3(0.0f, 0.0f, 0.0f) || SavedAngles == vec3(0.0f, 0.0f, 0.0f))
    {
        pMenu->SetFeedbackText("^1Save a position first!");
        return;
    }

    TeleportPlayer(GetEntity(iClientNum), reinterpret_cast<const float *>(&SavedPos), reinterpret_cast<const float *>(&SavedAngles));
}

void SpecOpsMW3MenuFunctions::ToggleUFO(Menu *pMenu)
{
    int iClientNum = pMenu->GetClientNum();

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

void SpecOpsMW3MenuFunctions::ToggleSecondPlayerGodMode(Menu *pMenu)
{
    // The second client num is always 1
    int iSecondClientNum = 1;

    // If the player name of the second client is empty, it means there is no second client
    gclient_s *pSecondClient = GetGClient(iSecondClientNum);
    if (!pSecondClient->connected)
    {
        pMenu->SetFeedbackText("^1No other player in the game!");
        return;
    }

    playerState_s *playerState = SV_GetPlayerstateForClientNum(iSecondClientNum);

    if (playerState->otherFlags == 0)
    {
        playerState->otherFlags = 1;
        pMenu->SetFeedbackText("Second Player God Mode ^2On");
    }
    else
    {
        playerState->otherFlags = 0;
        pMenu->SetFeedbackText("Second Player God Mode ^1Off");
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
        pMenu->SetFeedbackText("^1No other player in the game!");
        return;
    }

    // Get the first player's current position
    float fDistance = 100.0f;
    vec3 Origin = SV_GetPlayerstateForClientNum(iFirstClientNum)->origin;
    float fViewY = SV_GetPlayerstateForClientNum(iFirstClientNum)->viewAngles.y;

    // Teleport the second player in front of the first player
    pSecondClient->ps.origin = Math::ToFront(Origin, fViewY, fDistance);
}
