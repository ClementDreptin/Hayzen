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
    XASSERT(options.PatchAddress != 0);
    XASSERT(options.PatchValue != 0);
    XASSERT(options.DefaultValue != 0);

    Memory::Write<uint32_t>(options.PatchAddress, options.Enabled ? options.PatchValue : options.DefaultValue);

    return true;
}

bool ChangeJumpHeight(void *pParameters)
{
    XASSERT(pParameters != nullptr);

    uint32_t value = *reinterpret_cast<uint32_t *>(pParameters);

    // Set the new jump height value
    std::string command = "set jump_height " + std::to_string(static_cast<uint64_t>(value));
    Cbuf_AddText(0, command.c_str());

    return true;
}

static void PM_CheckLadderMoveHook(pmove_t *pm, void *pml)
{
    auto &detourMap = COMMON_TITLE::GetDetourMap();

    XASSERT(detourMap.find("PM_CheckLadderMove") != detourMap.end());

    pm->tracemask |= 0x10000;

    detourMap.at("PM_CheckLadderMove")->GetOriginal<decltype(&PM_CheckLadderMoveHook)>()(pm, pml);

    if (!(pm->ps->pm_flags & 8))
        pm->tracemask &= ~0x10000;
}

static void PmoveSingleHook(pmove_t *pm)
{
    auto &detourMap = COMMON_TITLE::GetDetourMap();

    XASSERT(detourMap.find("PmoveSingle") != detourMap.end());

    if (!(pm->ps->pm_flags & 8))
    {
        pm->tracemask &= ~0x10000;
        pm->tracemask |= 0x400;
    }

    detourMap.at("PmoveSingle")->GetOriginal<decltype(&PmoveSingleHook)>()(pm);
}

struct GoThroughInvisibleBarriersOptions
{
    bool Enabled;
    uintptr_t PM_CheckLadderMoveAddress;
    uintptr_t PmoveSingleAddress;
};

bool GoThroughInvisibleBarriers(const GoThroughInvisibleBarriersOptions &options)
{
    XASSERT(options.PM_CheckLadderMoveAddress != 0);
    XASSERT(options.PmoveSingleAddress != 0);

    auto &detourMap = COMMON_TITLE::GetDetourMap();

    if (options.Enabled)
    {
        // Patching pmove_t::tracemask in PmoveSingle prevent the player from climbing ladders so
        // we need to hook PM_CheckLadderMove to fix pmove_t::tracemask
        detourMap["PM_CheckLadderMove"] = new Detour(options.PM_CheckLadderMoveAddress, PM_CheckLadderMoveHook);
        detourMap["PmoveSingle"] = new Detour(options.PmoveSingleAddress, PmoveSingleHook);

        HRESULT hr1 = detourMap["PM_CheckLadderMove"]->Install();
        HRESULT hr2 = detourMap["PmoveSingle"]->Install();

        return SUCCEEDED(hr1) && SUCCEEDED(hr2);
    }
    else
    {
        delete detourMap["PM_CheckLadderMove"];
        delete detourMap["PmoveSingle"];

        detourMap.erase("PM_CheckLadderMove");
        detourMap.erase("PmoveSingle");

        return true;
    }
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
    XASSERT(pPlayerEntity != nullptr);

    SetClientOrigin(pPlayerEntity, reinterpret_cast<const float *>(&savedPosition));
    SetClientViewAngle(pPlayerEntity, reinterpret_cast<const float *>(&savedAngles));

    return true;
}

bool ToggleSaveLoadBinds(void *pParameters)
{
    XASSERT(pParameters != nullptr);

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
    XASSERT(pParameters != nullptr);

    bool enabled = *reinterpret_cast<bool *>(pParameters);

    gclient_s *pGClient = GetGClient(Context::ClientNum);
    XASSERT(pGClient != nullptr);

    pGClient->mFlags = enabled ? 2 : 0;

    return true;
}

}
