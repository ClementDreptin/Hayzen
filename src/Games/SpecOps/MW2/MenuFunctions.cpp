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
/*
void SpecOpsMW2::ToggleSaveLoadBinds(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::ToggleSaveLoadBinds(pMenu);
}

void SpecOpsMW2::SavePosition(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::SavePosition(pMenu);
}

void SpecOpsMW2::LoadPosition(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::LoadPosition(pMenu);
}

void SpecOpsMW2::ToggleUfo(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::ToggleUfo(pMenu);
}

void SpecOpsMW2::ToggleSecondPlayerGodMode(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::ToggleSecondPlayerGodMode(pMenu);
}

void SpecOpsMW2::TeleportSecondPlayerToMe(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::TeleportSecondPlayerToMe(pMenu);
}
*/
