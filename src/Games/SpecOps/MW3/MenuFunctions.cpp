#include "pch.h"
#include "Games\SpecOps\MW3\MenuFunctions.h"

using namespace SpecOpsMW3::Game;

#ifdef COMMON_FN_NAMESPACE
    #undef COMMON_FN_NAMESPACE
#endif
#define COMMON_FN_NAMESPACE SpecOpsMW3Common
#include "Games\Common\CommonFunctions.h"

#define GAME_MW3
#include "Games\Common\SpecOpsFunctions.h"
#undef GAME_MW3

void SpecOpsMW3::ToggleGodMode(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::ToggleGodModeSP(pMenu);
}

void SpecOpsMW3::ToggleAmmo(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::ToggleAmmoOptions options;
    options.pMenu = pMenu;
    options.patchAddress = 0x8235BB54;
    options.defaultValue = 0x7D3D5050;
    options.patchValue = 0x7D495378;

    COMMON_FN_NAMESPACE::ToggleAmmo(options);
}

void SpecOpsMW3::ChangeJumpHeight(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::ChangeJumpHeight(pMenu);
}

void SpecOpsMW3::ToggleSaveLoadBinds(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::ToggleSaveLoadBinds(pMenu);
}

void SpecOpsMW3::SavePosition(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::SavePosition(pMenu);
}

void SpecOpsMW3::LoadPosition(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::LoadPosition(pMenu);
}

void SpecOpsMW3::ToggleUfo(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::ToggleUfo(pMenu);
}

void SpecOpsMW3::ToggleSecondPlayerGodMode(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::ToggleSecondPlayerGodMode(pMenu);
}

void SpecOpsMW3::TeleportSecondPlayerToMe(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::TeleportSecondPlayerToMe(pMenu);
}
