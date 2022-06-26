#include "pch.h"
#include "Games\SpecOps\AlphaMW2\MenuFunctions.h"

using namespace SpecOpsAlphaMW2::Game;

#ifdef COMMON_FN_NAMESPACE
    #undef COMMON_FN_NAMESPACE
#endif
#define COMMON_FN_NAMESPACE SpecOpsAlphaMW2Common
#include "Games\Common\CommonFunctions.h"

#define GAME_ALPHAMW2
#include "Games\Common\SpecOpsFunctions.h"
#undef GAME_ALPHAMW2

void SpecOpsAlphaMW2::ToggleGodMode(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::ToggleGodModeSP(pMenu);
}

void SpecOpsAlphaMW2::ToggleAmmo(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::ToggleAmmoOptions Options;
    Options.pMenu = pMenu;
    Options.dwPatchAddress = 0x82328610;
    Options.dwDefaultValue = 0x7D1D4850;
    Options.dwPatchValue = 0x7D284B78;

    COMMON_FN_NAMESPACE::ToggleAmmo(Options);
}

void SpecOpsAlphaMW2::ChangeJumpHeight(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::ChangeJumpHeight(pMenu);
}

void SpecOpsAlphaMW2::ToggleSaveLoadBinds(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::ToggleSaveLoadBinds(pMenu);
}

void SpecOpsAlphaMW2::SavePosition(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::SavePosition(pMenu);
}

void SpecOpsAlphaMW2::LoadPosition(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::LoadPosition(pMenu);
}

void SpecOpsAlphaMW2::ToggleUfo(Menu *pMenu)
{
    Cbuf_AddText(0, "ufo");
}

void SpecOpsAlphaMW2::ToggleSecondPlayerGodMode(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::ToggleSecondPlayerGodMode(pMenu);
}

void SpecOpsAlphaMW2::TeleportSecondPlayerToMe(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::TeleportSecondPlayerToMe(pMenu);
}
