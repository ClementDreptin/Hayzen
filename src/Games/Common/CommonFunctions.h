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
    uintptr_t patchAddress;
    POWERPC_INSTRUCTION defaultValue;
    POWERPC_INSTRUCTION patchValue;
};

// Toggle unlimited ammo.
void ToggleAmmo(const ToggleAmmoOptions &options)
{
    int clientNum = options.pMenu->GetClientNum();

    if (Memory::Read<POWERPC_INSTRUCTION>(options.patchAddress) == options.defaultValue)
    {
        Memory::Write<POWERPC_INSTRUCTION>(options.patchAddress, options.patchValue);
        iPrintLn(clientNum, "Unlimited Ammo ^2On");
    }
    else
    {
        Memory::Write<POWERPC_INSTRUCTION>(options.patchAddress, options.defaultValue);
        iPrintLn(clientNum, "Unlimited Ammo ^1Off");
    }
}

// Toggle save and load binds.
void ToggleSaveLoadBinds(Menu *pMenu)
{
    int clientNum = pMenu->GetClientNum();

    if (!pMenu->BindsEnabled())
    {
        Cbuf_AddText(0, "unbind button_lshldr;unbind button_rshldr");
        iPrintLn(clientNum, "Press " CHAR_RB " to ^2Save^7 and " CHAR_LB " to ^2Load");
    }
    else
    {
        Cbuf_AddText(0, "bind button_lshldr \"+smoke\";bind button_rshldr \"+frag\"");
        iPrintLn(clientNum, "Save and Load binds ^1Off");
    }

    pMenu->ToggleBinds();
}

// Save the current player's position.
void SavePosition(Menu *pMenu)
{
    int clientNum = pMenu->GetClientNum();

    pMenu->SetSavedPosition(GetPlayerState(clientNum)->origin);
    pMenu->SetSavedAngles(GetPlayerState(clientNum)->viewAngles);

    iPrintLn(clientNum, "Position ^2Saved");
}

// Load the previously saved player's position.
void LoadPosition(Menu *pMenu)
{
    int clientNum = pMenu->GetClientNum();

    const vec3 &savedPosition = pMenu->GetSavedPosition();
    const vec3 &savedAngles = pMenu->GetSavedAngles();

    // Make sure the player previously saved their position
    if (savedPosition.isNull() || savedAngles.isNull())
    {
        iPrintLn(clientNum, "^1Save a position first!");
        return;
    }

    gentity_s *pPlayerEntity = GetEntity(clientNum);

    SetClientOrigin(pPlayerEntity, reinterpret_cast<const float *>(&savedPosition));
    SetClientViewAngle(pPlayerEntity, reinterpret_cast<const float *>(&savedAngles));
}

// Toggle UFO.
void ToggleUfo(Menu *pMenu)
{
    int clientNum = pMenu->GetClientNum();

    gclient_s *pGClient = GetGClient(clientNum);

    // The default value of mFlags is 0, the UFO value is 2 so we just need to toggle the second bit
    // to toggle UFO
    BIT_FLIP(pGClient->mFlags, 1);

    iPrintLn(clientNum, BIT_CHECK(pGClient->mFlags, 1) ? "Ufo ^2On" : "Ufo ^1Off");
}

}
