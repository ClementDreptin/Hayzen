#include "pch.h"
#include "Games/MW3/MenuFunctions.h"

#include "Games/MW3/GameFunctions.h"
#include "Games/MW3/MW3Title.h"

using namespace MW3::Game;

#ifdef COMMON_FN_NAMESPACE
    #undef COMMON_FN_NAMESPACE
#endif
#define COMMON_FN_NAMESPACE MW3Common
#ifdef COMMON_TITLE
    #undef COMMON_TITLE
#endif
#define COMMON_TITLE MW3Title
#include "Games/Common/CommonFunctions.h"

#define GAME_MW3
#include "Games/Common/MultiplayerFunctions.h"
#undef GAME_MW3

bool MW3::ToggleGodMode(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleGodModeMP(pParameters);
}

bool MW3::ToggleFallDamage(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleFallDamage(pParameters, 0x82000C04);
}

bool MW3::ToggleAmmo(void *pParameters)
{
    XASSERT(pParameters != nullptr);

    bool enabled = *reinterpret_cast<bool *>(pParameters);

    COMMON_FN_NAMESPACE::ToggleAmmoOptions options = {};
    options.Enabled = enabled;
    options.PatchAddress = 0x820F63E4;
    options.DefaultValue = 0x7D3D5050;
    options.PatchValue = 0x7D495378;

    return COMMON_FN_NAMESPACE::ToggleAmmo(options);
}

bool MW3::GoThroughInvisibleBarriers(void *pParameters)
{
    XASSERT(pParameters != nullptr);

    bool enabled = *reinterpret_cast<bool *>(pParameters);

    COMMON_FN_NAMESPACE::GoThroughInvisibleBarriersOptions options = {};
    options.Enabled = enabled;
    options.PM_CheckLadderMoveAddress = 0x820E94A0;
    options.PmoveSingleAddress = 0x820E9C68;

    return COMMON_FN_NAMESPACE::GoThroughInvisibleBarriers(options);
}

static bool PM_IsAdsAllowedHook(pmove_t *pm, pml_t *pml)
{
    auto &detourMap = COMMON_TITLE::GetDetourMap();

    XASSERT(detourMap.find("PM_IsAdsAllowed") != detourMap.end());

    if (pm->ps->weapState[0].weapAnim == WEAP_FORCE_IDLE && !pm->ps->weapEquippedData[0].dualWielding)
        return true;

    return detourMap.at("PM_IsAdsAllowed").GetOriginal<decltype(&PM_IsAdsAllowedHook)>()(pm, pml);
}

static void PM_Weapon_FireWeaponHook(pmove_t *pm, int delayedAction, uint32_t *holdrand, uint32_t hand)
{
    auto &detourMap = COMMON_TITLE::GetDetourMap();

    if ((pm->cmd.buttons & 0x20) && PM_Weapon_AllowReload(pm->ps, hand))
    {
        if (pm->ps->weapState[hand].weapAnim == WEAP_RELOAD || pm->ps->weapState[hand].weapAnim == WEAP_RELOAD_START)
            return;
    }

    detourMap.at("PM_Weapon_FireWeapon").GetOriginal<decltype(&PM_Weapon_FireWeaponHook)>()(pm, delayedAction, holdrand, hand);
}

static void PM_WeaponProcessHandHook(pmove_t *pm, pml_t *pml, int delayedAction, uint32_t hand)
{
    auto &detourMap = COMMON_TITLE::GetDetourMap();

    XASSERT(detourMap.find("PM_WeaponProcessHand") != detourMap.end());

    auto original = detourMap.at("PM_WeaponProcessHand").GetOriginal<decltype(&PM_WeaponProcessHandHook)>();

    if (pm->ps->weapState[hand].weaponState == WEAPON_FIRING)
    {
        original(pm, pml, delayedAction, hand);
        return;
    }

    if ((pm->cmd.buttons & 0x1) &&
        (pm->cmd.buttons & 0x20) &&
        !pm->ps->weapEquippedData[0].dualWielding &&
        pm->ps->weapState[hand].weaponState != WEAPON_RECHAMBERING &&
        pm->ps->weapState[hand].weapAnim == WEAP_FORCE_IDLE)
    {
        pm->ps->weapState[hand].weapAnim = BG_SegmentedReload(pm->ps->weapCommon.weapon, false) ? WEAP_RELOAD_START : WEAP_RELOAD;
        pm->ps->weapCommon.weapFlags = 0;

        PM_Weapon_FireWeaponHook(pm, 0, &pml->holdrand, hand);
        return;
    }

    if (pm->ps->weapState[hand].weapAnim == WEAP_FORCE_IDLE)
    {
        pm->ps->weapState[hand].weaponRestrictKickTime = 1;
        pm->ps->weapState[hand].weaponState = WEAPON_READY;
        pm->ps->weapState[hand].weaponTime = 0;
        pm->ps->weapState[hand].weaponDelay = 0;
    }

    original(pm, pml, delayedAction, hand);
}

bool MW3::ToggleDoubleTaps(void *pParameters)
{
    XASSERT(pParameters != nullptr);

    bool enabled = *reinterpret_cast<bool *>(pParameters);

    auto &detourMap = COMMON_TITLE::GetDetourMap();

    if (enabled)
    {
        detourMap["PM_IsAdsAllowed"] = Detour(0x820F3740, PM_IsAdsAllowedHook);
        detourMap["PM_Weapon_FireWeapon"] = Detour(0x820F17B0, PM_Weapon_FireWeaponHook);
        detourMap["PM_WeaponProcessHand"] = Detour(0x820F5998, PM_WeaponProcessHandHook);

        HRESULT hr1 = detourMap["PM_IsAdsAllowed"].Install();
        HRESULT hr2 = detourMap["PM_Weapon_FireWeapon"].Install();
        HRESULT hr3 = detourMap["PM_WeaponProcessHand"].Install();

        return SUCCEEDED(hr1) && SUCCEEDED(hr2) && SUCCEEDED(hr3);
    }
    else
    {
        detourMap.erase("PM_IsAdsAllowed");
        detourMap.erase("PM_Weapon_FireWeapon");
        detourMap.erase("PM_WeaponProcessHand");

        return true;
    }
}

bool MW3::RecordInput(void *pParameters)
{
    return COMMON_FN_NAMESPACE::RecordInput(pParameters);
}

bool MW3::ToggleReplayInputBind(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleReplayInputBind(pParameters);
}

bool MW3::SpawnCarePackage(void *)
{
    return COMMON_FN_NAMESPACE::SpawnCarePackage();
}

bool MW3::SpawnBlocker(void *)
{
    return COMMON_FN_NAMESPACE::SpawnBlocker();
}

bool MW3::ChangeCarePackagePositionPresets(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ChangeCarePackagePositionPresets(pParameters);
}

bool MW3::ChangeCarePackageOrientation(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ChangeCarePackageOrientation(pParameters);
}

bool MW3::ToggleSaveLoadBinds(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleSaveLoadBinds(pParameters);
}

bool MW3::ToggleUfoBind(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleUfoBind(pParameters);
}
