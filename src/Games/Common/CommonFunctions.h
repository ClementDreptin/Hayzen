// #pragma once is intentionally missing, this file is supposed to be included multiple times
// and generating different functions every time depending on the COMMON_FN_NAMESPACE macro.

#include "Core\Menu.h"
#include "Core\Bits.h"


namespace COMMON_FN_NAMESPACE
{

// Options passed to the ToggleAmmo function.
struct ToggleAmmoOptions
{
    Menu *pMenu;
    DWORD dwPatchAddress;
    DWORD dwDefaultValue;
    DWORD dwPatchValue;
};

// Toggle unlimited ammo.
void ToggleAmmo(const ToggleAmmoOptions &Options)
{
    int iClientNum = Options.pMenu->GetClientNum();

    if (Memory::Read<DWORD>(Options.dwPatchAddress) == Options.dwDefaultValue)
    {
        Memory::Write<DWORD>(Options.dwPatchAddress, Options.dwPatchValue);
        iPrintLn(iClientNum, "Unlimited Ammo ^2On");
    }
    else
    {
        Memory::Write<DWORD>(Options.dwPatchAddress, Options.dwDefaultValue);
        iPrintLn(iClientNum, "Unlimited Ammo ^1Off");
    }
}

// Toggle save and load binds.
void ToggleSaveLoadBinds(Menu *pMenu)
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

// Save the current player's position.
void SavePosition(Menu *pMenu)
{
    int iClientNum = pMenu->GetClientNum();

    pMenu->SetSavedPos(GetPlayerState(iClientNum)->origin);
    pMenu->SetSavedAngles(GetPlayerState(iClientNum)->viewAngles);

    iPrintLn(iClientNum, "Position ^2Saved");
}

// Load the previously saved player's position.
void LoadPosition(Menu *pMenu)
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

// Toggle UFO.
void ToggleUfo(Menu *pMenu)
{
    int iClientNum = pMenu->GetClientNum();

    gclient_s *pGClient = GetGClient(iClientNum);

    // The default value of mFlags is 0, the UFO value is 2 so we just need to toggle the second bit
    // to toggle UFO
    BIT_FLIP(pGClient->mFlags, 1);

    iPrintLn(iClientNum, BIT_CHECK(pGClient->mFlags, 1) ? "Ufo ^2On" : "Ufo ^1Off");
}

}
