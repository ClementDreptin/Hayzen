// #pragma once is intentionally missing, this file is supposed to be included multiple times
// and generating different functions every time depending on the COMMON_FN_NAMESPACE macro.

#include "Core/Context.h"
#include "Core/UI.h"

namespace COMMON_FN_NAMESPACE
{

struct ToggleAmmoOptions
{
    bool Enabled;
    uintptr_t PatchAddress;
    uint32_t DefaultValue;
    uint32_t PatchValue;
};

bool ToggleAmmo(const ToggleAmmoOptions &options)
{
    Memory::Write<uint32_t>(options.PatchAddress, options.Enabled ? options.PatchValue : options.DefaultValue);

    return true;
}

bool ChangeJumpHeight(void *pParameters)
{
    uint32_t value = *reinterpret_cast<uint32_t *>(pParameters);

    // Set the new jump height value
    std::string command = "set jump_height " + std::to_string(static_cast<uint64_t>(value));
    Cbuf_AddText(0, command.c_str());

    return true;
}

bool SavePosition()
{
    int clientNum = Context::ClientNum;

    Context::SavedPosition = GetPlayerState(clientNum)->origin;
    Context::SavedAngles = GetPlayerState(clientNum)->viewAngles;

    iPrintLn(clientNum, "Position ^2Saved");

    return true;
}

bool LoadPosition()
{
    int clientNum = Context::ClientNum;

    const vec3 &savedPosition = Context::SavedPosition;
    const vec3 &savedAngles = Context::SavedAngles;

    // Make sure the player previously saved their position
    if (savedPosition.isNull() || savedAngles.isNull())
    {
        iPrintLn(clientNum, "^1Save a position first!");
        return false;
    }

    gentity_s *pPlayerEntity = GetEntity(clientNum);

    SetClientOrigin(pPlayerEntity, reinterpret_cast<const float *>(&savedPosition));
    SetClientViewAngle(pPlayerEntity, reinterpret_cast<const float *>(&savedAngles));

    return true;
}

bool ToggleSaveLoadBinds(void *pParameters)
{
    bool enabled = *reinterpret_cast<bool *>(pParameters);

    if (enabled)
    {
        Cbuf_AddText(0, "unbind button_lshldr;unbind button_rshldr");
        iPrintLn(Context::ClientNum, "Press " CHAR_RB " to ^2Save^7 and " CHAR_LB " to ^2Load");
    }
    else
        Cbuf_AddText(0, "bind button_lshldr \"+smoke\";bind button_rshldr \"+frag\"");

    return true;
}

bool ToggleUfo(void *pParameters)
{
    bool enabled = *reinterpret_cast<bool *>(pParameters);

    gclient_s *pGClient = GetGClient(Context::ClientNum);

    pGClient->mFlags = enabled ? 2 : 0;

    return true;
}

}
