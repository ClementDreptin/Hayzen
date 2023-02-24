#include "pch.h"
#include "Games/SpecOps/MW2/MenuFunctions.h"

using namespace SpecOpsMW2::Game;

#ifdef COMMON_FN_NAMESPACE
    #undef COMMON_FN_NAMESPACE
#endif
#define COMMON_FN_NAMESPACE SpecOpsMW2Common
#include "Games/Common/CommonFunctions.h"

#define GAME_MW2
#include "Games/Common/SpecOpsFunctions.h"
#undef GAME_MW2

bool SpecOpsMW2::ToggleGodMode(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleGodModeSP(pParameters);
}

bool SpecOpsMW2::ToggleAmmo(void *pParameters)
{
    bool enabled = *reinterpret_cast<bool *>(pParameters);

    COMMON_FN_NAMESPACE::ToggleAmmoOptions options = { 0 };
    options.Enabled = enabled;
    options.PatchAddress = 0x82331F48;
    options.DefaultValue = 0x7D1D4850;
    options.PatchValue = 0x7D284B78;

    return COMMON_FN_NAMESPACE::ToggleAmmo(options);
}

bool SpecOpsMW2::ChangeJumpHeight(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ChangeJumpHeight(pParameters);
}

bool SpecOpsMW2::ToggleSaveLoadBinds(void *)
{
    return COMMON_FN_NAMESPACE::ToggleSaveLoadBinds();
}

bool SpecOpsMW2::SavePosition(void *)
{
    return COMMON_FN_NAMESPACE::SavePosition();
}

bool SpecOpsMW2::LoadPosition(void *)
{
    return COMMON_FN_NAMESPACE::LoadPosition();
}

bool SpecOpsMW2::ToggleUfo(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleUfo(pParameters);
}

bool SpecOpsMW2::ToggleSecondPlayerGodMode(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleSecondPlayerGodMode(pParameters);
}

bool SpecOpsMW2::TeleportSecondPlayerToMe(void *)
{
    return COMMON_FN_NAMESPACE::TeleportSecondPlayerToMe();
}
