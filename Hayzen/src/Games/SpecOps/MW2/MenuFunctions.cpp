#include "pch.h"
#include "Games\SpecOps\MW2\MenuFunctions.h"

using namespace SpecOpsMW2GameFunctions;


VOID SpecOpsMW2MenuFunctions::ToggleGodMode(Menu *pMenu)
{
    INT iClientNum = pMenu->GetClientNum();
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

VOID SpecOpsMW2MenuFunctions::ToggleAmmo(Menu *pMenu)
{
    DWORD dwAddress = 0x82331F48;
    DWORD dwDefaultValue = 0x7D1D4850;
    DWORD dwModifiedValue = 0x7D284B78;

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

VOID SpecOpsMW2MenuFunctions::ToggleSaveLoadBinds(Menu *pMenu)
{
    if (!pMenu->BindsEnabled())
        pMenu->SetFeedbackText("Press " CHAR_RB " to ^2Save^7 and " CHAR_LB " to ^2Load");
    else
        pMenu->SetFeedbackText("Save and Load binds ^1Off");

    pMenu->ToggleBinds();
}

VOID SpecOpsMW2MenuFunctions::SavePosition(Menu *pMenu)
{
    INT iClientNum = pMenu->GetClientNum();

    pMenu->SetSavedPos(SV_GetPlayerstateForClientNum(iClientNum)->origin);
    pMenu->SetSavedAngles(SV_GetPlayerstateForClientNum(iClientNum)->viewAngles);

    pMenu->SetFeedbackText("Position ^2Saved");
}

VOID SpecOpsMW2MenuFunctions::LoadPosition(Menu *pMenu)
{
    INT iClientNum = pMenu->GetClientNum();
    CONST vec3 &SavedPos = pMenu->GetSavedPos();
    CONST vec3 &SavedAngles = pMenu->GetSavedAngles();

    // Make sure the player previously saved their position
    if (SavedPos == vec3(0.0f, 0.0f, 0.0f) || SavedAngles == vec3(0.0f, 0.0f, 0.0f))
    {
        pMenu->SetFeedbackText("^1Save a position first!");
        return;
    }

    TeleportPlayer(GetEntity(iClientNum), (PFLOAT)&SavedPos, (PFLOAT)&SavedAngles);
}

VOID SpecOpsMW2MenuFunctions::ToggleUFO(Menu *pMenu)
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

VOID SpecOpsMW2MenuFunctions::ToggleSecondPlayerGodMode(Menu *pMenu)
{
    // The second client num is always 1
    INT iSecondClientNum = 1;

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

VOID SpecOpsMW2MenuFunctions::TeleportSecondPlayerToMe(Menu *pMenu)
{
    // The second client num is always 1
    INT iSecondClientNum = 1;
    INT iFirstClientNum = pMenu->GetClientNum();

    gclient_s *pSecondClient = GetGClient(iSecondClientNum);
    if (!pSecondClient->connected)
    {
        pMenu->SetFeedbackText("^1No other player in the game!");
        return;
    }

    // Get the first player's current position
    FLOAT fDistance = 100.0f;
    vec3 Origin = SV_GetPlayerstateForClientNum(iFirstClientNum)->origin;
    FLOAT fViewY = SV_GetPlayerstateForClientNum(iFirstClientNum)->viewAngles.y;

    // Teleport the second player in front of the first player
    pSecondClient->ps.origin = Math::ToFront(Origin, fViewY, fDistance);
}
