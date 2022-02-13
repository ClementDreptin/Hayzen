#include "pch.h"
#include "Games\SpecOps\MW2\MenuFunctions.h"

using namespace SpecOpsMW2GameFunctions;


void SpecOpsMW2MenuFunctions::ToggleGodMode(Menu *pMenu)
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

void SpecOpsMW2MenuFunctions::ToggleAmmo(Menu *pMenu)
{
    int iClientNum = pMenu->GetClientNum();

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

// Threaded function that prompts a keyboard and sets the jump height value to what was entered.
static DWORD ChangeJumpHeightThread(Menu *pMenu)
{
    // Get the value from the user via the virtual keyboard
    std::string strValue;
    DWORD dwResult = Xam::ShowKeyboard("Jump Height", "Max value: 999\nDefault value: 39", "39", strValue, 3, VKBD_LATIN_NUMERIC);

    // If the user canceled the keyboard, return early
    if (dwResult != ERROR_SUCCESS)
        return 0;

    // If the user did not enter anything but still closed the keyboard by pressing START, set the value to its default value
    if (strValue == "")
        strValue = "39";

    // Set the new jump height value
    std::string strFullCommand = "set jump_height " + strValue;
    Cbuf_AddText(0, strFullCommand.c_str());

    iPrintLn(pMenu->GetClientNum(), "Jump Height set to ^2" + strValue);

    return 0;
}

void SpecOpsMW2MenuFunctions::ChangeJumpHeight(Menu *pMenu)
{
    // This needs to execute on a separate thread because we need to wait for the user
    // to finish typing. If this wasn't done on a separate thread, it would block the
    // game's thread and make it crash.
    Memory::Thread(reinterpret_cast<PTHREAD_START_ROUTINE>(ChangeJumpHeightThread), pMenu);
}

void SpecOpsMW2MenuFunctions::ToggleSaveLoadBinds(Menu *pMenu)
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

void SpecOpsMW2MenuFunctions::SavePosition(Menu *pMenu)
{
    int iClientNum = pMenu->GetClientNum();

    pMenu->SetSavedPos(SV_GetPlayerstateForClientNum(iClientNum)->origin);
    pMenu->SetSavedAngles(SV_GetPlayerstateForClientNum(iClientNum)->viewAngles);

    iPrintLn(iClientNum, "Position ^2Saved");
}

void SpecOpsMW2MenuFunctions::LoadPosition(Menu *pMenu)
{
    int iClientNum = pMenu->GetClientNum();

    const vec3 &SavedPos = pMenu->GetSavedPos();
    const vec3 &SavedAngles = pMenu->GetSavedAngles();

    // Make sure the player previously saved their position
    if (SavedPos.isNull() || SavedAngles.isNull())
    {
        iPrintLn(iClientNum, "^1Save a position first!");
        return;
    }

    gentity_s *pPlayerEntity = GetEntity(iClientNum);

    SetClientOrigin(pPlayerEntity, reinterpret_cast<const float *>(&SavedPos));
    SetClientViewAngle(pPlayerEntity, reinterpret_cast<const float *>(&SavedAngles));
}

void SpecOpsMW2MenuFunctions::ToggleUFO(Menu *pMenu)
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

void SpecOpsMW2MenuFunctions::ToggleSecondPlayerGodMode(Menu *pMenu)
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

void SpecOpsMW2MenuFunctions::TeleportSecondPlayerToMe(Menu *pMenu)
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
