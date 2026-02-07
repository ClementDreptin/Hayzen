// #pragma once is intentionally missing, this file is supposed to be included multiple times
// and generating different functions every time depending on the COMMON_FN_NAMESPACE macro.

#include "Core/Context.h"
#include "Core/UI.h"
#include "Modules/Binds.h"
#include "Modules/InputRecorder.h"

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

static void PM_CheckLadderMoveHook(pmove_t *pm, void *pml)
{
    auto &detourMap = COMMON_TITLE::GetDetourMap();

    XASSERT(detourMap.find("PM_CheckLadderMove") != detourMap.end());

    pm->tracemask |= 0x10000;

    detourMap.at("PM_CheckLadderMove").GetOriginal<decltype(&PM_CheckLadderMoveHook)>()(pm, pml);

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

    detourMap.at("PmoveSingle").GetOriginal<decltype(&PmoveSingleHook)>()(pm);
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
        // Patching pmove_t::tracemask in PmoveSingle prevents the player from climbing ladders so
        // we need to hook PM_CheckLadderMove to fix pmove_t::tracemask
        detourMap["PM_CheckLadderMove"] = Detour(options.PM_CheckLadderMoveAddress, PM_CheckLadderMoveHook);
        detourMap["PmoveSingle"] = Detour(options.PmoveSingleAddress, PmoveSingleHook);

        HRESULT hr1 = detourMap["PM_CheckLadderMove"].Install();
        HRESULT hr2 = detourMap["PmoveSingle"].Install();

        return SUCCEEDED(hr1) && SUCCEEDED(hr2);
    }
    else
    {
        detourMap.erase("PM_CheckLadderMove");
        detourMap.erase("PmoveSingle");

        return true;
    }
}

static bool SavePosition()
{
    int clientNum = Context::ClientNum;

    Context::SavedPosition = GetPlayerState(clientNum)->origin;
    Context::SavedAngles = GetPlayerState(clientNum)->viewAngles;

    iPrintLn(clientNum, "Position ^2Saved");

    return true;
}

static bool LoadPosition()
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

        Binds::Add(XINPUT_GAMEPAD_LEFT_SHOULDER, LoadPosition);
        Binds::Add(XINPUT_GAMEPAD_RIGHT_SHOULDER, SavePosition);

        iPrintLn(Context::ClientNum, "Press " CHAR_RB " to ^2Save^7 and " CHAR_LB " to ^2Load");
    }
    else
    {
        Cbuf_AddText(0, "bind button_lshldr \"+smoke\";bind button_rshldr \"+frag\"");

        Binds::Remove(XINPUT_GAMEPAD_LEFT_SHOULDER);
        Binds::Remove(XINPUT_GAMEPAD_RIGHT_SHOULDER);
    }

    return true;
}

static bool ToggleUfo()
{
    gclient_s *pGClient = GetGClient(Context::ClientNum);
    XASSERT(pGClient != nullptr);

    // Avoid activating UFO while using the start or class menu
    if (UI_AnyMenuActive(0))
        return false;

    pGClient->mFlags ^= 2;

    return true;
}

bool ToggleUfoBind(void *pParameters)
{
    XASSERT(pParameters != nullptr);

    bool enabled = *reinterpret_cast<bool *>(pParameters);

    if (enabled)
    {
        Cbuf_AddText(0, "unbind dpad_up");
        Binds::Add(XINPUT_GAMEPAD_DPAD_UP, ToggleUfo);

        iPrintLn(Context::ClientNum, "Press ^2UP^7 to ^2UFO");
    }
    else
    {
        Cbuf_AddText(0, "bind dpad_up \"+actionslot 1\"");
        Binds::Remove(XINPUT_GAMEPAD_DPAD_UP);
    }

    return true;
}

bool RecordInput(void *pParameters)
{
    XASSERT(pParameters != nullptr);

    bool enabled = *reinterpret_cast<bool *>(pParameters);

    int clientNum = Context::ClientNum;

    if (enabled)
    {
        InputRecorder::Start();
        iPrintLn(clientNum, "Recording...");
    }
    else
    {
        InputRecorder::Stop();
        iPrintLn(clientNum, "Recording stopped");
    }

    return true;
}

static bool ReplayInput()
{
    // Avoid replaying while using the start or class menu
    if (UI_AnyMenuActive(0))
        return false;

    InputRecorder::Replay();

    return true;
}

bool ToggleReplayInputBind(void *pParameters)
{
    XASSERT(pParameters != nullptr);

    bool enabled = *reinterpret_cast<bool *>(pParameters);

    if (enabled)
    {
        Binds::Add(XINPUT_GAMEPAD_DPAD_DOWN, ReplayInput);

        // It'd be nice to display the button glyph instead of "Down" but CHAR_DOWN (\x15) isn't in the font
        // used by iPrintLn and button macros, like "[{+actionslot 2}]", don't work in spec ops
        iPrintLn(Context::ClientNum, "Press ^2DOWN^7 to ^2Replay");
    }
    else
        Binds::Remove(XINPUT_GAMEPAD_DPAD_DOWN);

    return true;
}

}
