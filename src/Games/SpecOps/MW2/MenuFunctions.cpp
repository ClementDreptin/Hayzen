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

void SpecOpsMW2::ToggleGodMode(void *pParameters)
{
    COMMON_FN_NAMESPACE::ToggleGodModeSP(pParameters);
}

void SpecOpsMW2::ToggleAmmo(void *pParameters)
{
    COMMON_FN_NAMESPACE::ToggleAmmoOptions options = { 0 };
    options.PatchAddress = 0x82331F48;
    options.DefaultValue = 0x7D1D4850;
    options.PatchValue = 0x7D284B78;

    COMMON_FN_NAMESPACE::ToggleAmmo(options);
}

void SpecOpsMW2::ChangeJumpHeight(void *pParameters)
{
    COMMON_FN_NAMESPACE::ChangeJumpHeight(pParameters);
}

void SpecOpsMW2::ToggleSaveLoadBinds(void *)
{
    COMMON_FN_NAMESPACE::ToggleSaveLoadBinds();
}

void SpecOpsMW2::SavePosition(void *)
{
    COMMON_FN_NAMESPACE::SavePosition();
}

void SpecOpsMW2::LoadPosition(void *)
{
    COMMON_FN_NAMESPACE::LoadPosition();
}

void SpecOpsMW2::ToggleUfo(void *pParameters)
{
    COMMON_FN_NAMESPACE::ToggleUfo(pParameters);
}

void SpecOpsMW2::ToggleSecondPlayerGodMode(void *pParameters)
{
    COMMON_FN_NAMESPACE::ToggleSecondPlayerGodMode(pParameters);
}

void SpecOpsMW2::TeleportSecondPlayerToMe(void *)
{
    COMMON_FN_NAMESPACE::TeleportSecondPlayerToMe();
}
