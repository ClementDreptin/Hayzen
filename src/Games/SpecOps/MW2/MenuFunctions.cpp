#include "pch.h"
#include "Games/SpecOps/MW2/MenuFunctions.h"

#include "Games/SpecOps/MW2/GameFunctions.h"
#include "Games/SpecOps/MW2/SpecOpsMW2Title.h"

using namespace SpecOpsMW2::Game;

#ifdef COMMON_FN_NAMESPACE
    #undef COMMON_FN_NAMESPACE
#endif
#define COMMON_FN_NAMESPACE SpecOpsMW2Common
#ifdef COMMON_TITLE
    #undef COMMON_TITLE
#endif
#define COMMON_TITLE SpecOpsMW2Title
#include "Games/Common/CommonFunctions.h"

#define GAME_MW2
#include "Games/Common/SpecOpsFunctions.h"
#undef GAME_MW2

bool SpecOpsMW2::ToggleGodMode(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleGodModeSP(enabled);
}

bool SpecOpsMW2::ToggleAmmo(bool enabled)
{
    COMMON_FN_NAMESPACE::ToggleAmmoOptions options = {};
    options.Enabled = enabled;
    options.PatchAddress = 0x82331F48;
    options.DefaultValue = 0x7D1D4850;
    options.PatchValue = 0x7D284B78;

    return COMMON_FN_NAMESPACE::ToggleAmmo(options);
}

bool SpecOpsMW2::ChangeJumpHeight(uint32_t value)
{
    return COMMON_FN_NAMESPACE::ChangeJumpHeight(value);
}

bool SpecOpsMW2::GoThroughInvisibleBarriers(bool enabled)
{
    COMMON_FN_NAMESPACE::GoThroughInvisibleBarriersOptions options = {};
    options.Enabled = enabled;
    options.PM_CheckLadderMoveAddress = 0x82329450;
    options.PmoveSingleAddress = 0x82329C20;

    return COMMON_FN_NAMESPACE::GoThroughInvisibleBarriers(options);
}

bool SpecOpsMW2::ToggleSaveLoadBinds(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleSaveLoadBinds(enabled);
}

bool SpecOpsMW2::ToggleUfoBind(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleUfoBind(enabled);
}

bool SpecOpsMW2::ToggleSecondPlayerGodMode(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleSecondPlayerGodMode(enabled);
}

void SpecOpsMW2::TeleportSecondPlayerToMe()
{
    COMMON_FN_NAMESPACE::TeleportSecondPlayerToMe();
}

bool SpecOpsMW2::RecordInput(bool enabled)
{
    return COMMON_FN_NAMESPACE::RecordInput(enabled);
}

bool SpecOpsMW2::ToggleReplayInputBind(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleReplayInputBind(enabled);
}
