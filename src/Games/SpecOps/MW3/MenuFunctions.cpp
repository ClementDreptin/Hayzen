#include "pch.h"
#include "Games/SpecOps/MW3/MenuFunctions.h"

#include "Games/SpecOps/MW3/GameFunctions.h"
#include "Games/SpecOps/MW3/SpecOpsMW3Title.h"

using namespace SpecOpsMW3::Game;

#ifdef COMMON_FN_NAMESPACE
    #undef COMMON_FN_NAMESPACE
#endif
#define COMMON_FN_NAMESPACE SpecOpsMW3Common
#ifdef COMMON_TITLE
    #undef COMMON_TITLE
#endif
#define COMMON_TITLE SpecOpsMW3Title
#include "Games/Common/CommonFunctions.h"

#define GAME_MW3
#include "Games/Common/SpecOpsFunctions.h"
#undef GAME_MW3

bool SpecOpsMW3::ToggleGodMode(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleGodModeSP(enabled);
}

bool SpecOpsMW3::ToggleAmmo(bool enabled)
{
    COMMON_FN_NAMESPACE::ToggleAmmoOptions options = {};
    options.Enabled = enabled;
    options.PatchAddress = 0x8235BB54;
    options.DefaultValue = 0x7D3D5050;
    options.PatchValue = 0x7D495378;

    return COMMON_FN_NAMESPACE::ToggleAmmo(options);
}

bool SpecOpsMW3::ChangeJumpHeight(uint32_t value)
{
    return COMMON_FN_NAMESPACE::ChangeJumpHeight(value);
}

bool SpecOpsMW3::GoThroughInvisibleBarriers(bool enabled)
{
    COMMON_FN_NAMESPACE::GoThroughInvisibleBarriersOptions options = {};
    options.Enabled = enabled;
    options.PM_CheckLadderMoveAddress = 0x82351650;
    options.PmoveSingleAddress = 0x82351E58;

    return COMMON_FN_NAMESPACE::GoThroughInvisibleBarriers(options);
}

bool SpecOpsMW3::ToggleSaveLoadBinds(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleSaveLoadBinds(enabled);
}

bool SpecOpsMW3::ToggleUfoBind(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleUfoBind(enabled);
}

bool SpecOpsMW3::ToggleSecondPlayerGodMode(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleSecondPlayerGodMode(enabled);
}

void SpecOpsMW3::TeleportSecondPlayerToMe()
{
    COMMON_FN_NAMESPACE::TeleportSecondPlayerToMe();
}

bool SpecOpsMW3::RecordInput(bool enabled)
{
    return COMMON_FN_NAMESPACE::RecordInput(enabled);
}

bool SpecOpsMW3::ToggleReplayInputBind(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleReplayInputBind(enabled);
}
