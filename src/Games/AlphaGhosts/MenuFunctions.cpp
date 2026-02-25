#include "pch.h"
#include "Games/AlphaGhosts/MenuFunctions.h"

#include "Games/AlphaGhosts/AlphaGhostsTitle.h"
#include "Games/AlphaGhosts/GameFunctions.h"

using namespace AlphaGhosts::Game;

#ifdef COMMON_FN_NAMESPACE
    #undef COMMON_FN_NAMESPACE
#endif
#define COMMON_FN_NAMESPACE AlphaGhostsCommon
#ifdef COMMON_TITLE
    #undef COMMON_TITLE
#endif
#define COMMON_TITLE AlphaGhostsTitle
#include "Games/Common/CommonFunctions.h"

#define GAME_ALPHAGHOSTS
#include "Games/Common/MultiplayerFunctions.h"
#undef GAME_ALPHAGHOSTS

bool AlphaGhosts::ToggleGodMode(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleGodModeMP(enabled);
}

bool AlphaGhosts::ToggleFallDamage(bool enabled)
{
    // For the Ghosts Alpha we can't use the common function because changing the constant value
    // doesn't work so we went back to the old dvar way

    if (enabled)
    {
        Cbuf_AddText(0, "set bg_fallDamageMinHeight 9998");
        Cbuf_AddText(0, "set bg_fallDamageMaxHeight 9999");
    }
    else
    {
        Cbuf_AddText(0, "set bg_fallDamageMinHeight 128");
        Cbuf_AddText(0, "set bg_fallDamageMaxHeight 300");
    }

    return true;
}

bool AlphaGhosts::ToggleAmmo(bool enabled)
{
    COMMON_FN_NAMESPACE::ToggleAmmoOptions options = {};
    options.Enabled = enabled;
    options.PatchAddress = 0x823A1234;
    options.DefaultValue = 0x4BFFFDDD;
    options.PatchValue = 0x60000000;

    return COMMON_FN_NAMESPACE::ToggleAmmo(options);
}

bool AlphaGhosts::ChangeJumpHeight(uint32_t value)
{
    // For the Ghosts Alpha changing the constant dvar value doesn't work
    // so we went back to the old dvar way

    Cbuf_AddText(0, Formatter::Format("set jump_height %d", value).c_str());

    return true;
}

bool AlphaGhosts::GoThroughInvisibleBarriers(bool enabled)
{
    COMMON_FN_NAMESPACE::GoThroughInvisibleBarriersOptions options = {};
    options.Enabled = enabled;
    options.PM_CheckLadderMoveAddress = 0x82387F70;
    options.PmoveSingleAddress = 0x8238AF00;

    return COMMON_FN_NAMESPACE::GoThroughInvisibleBarriers(options);
}

void AlphaGhosts::SpawnCrate()
{
    COMMON_FN_NAMESPACE::SpawnCrate();
}

void AlphaGhosts::SpawnBlocker()
{
    COMMON_FN_NAMESPACE::SpawnBlocker();
}

bool AlphaGhosts::ChangeCratePositionPresets(size_t index)
{
    return COMMON_FN_NAMESPACE::ChangeCratePositionPresets(index);
}

bool AlphaGhosts::ChangeCrateOrientation(size_t index)
{
    return COMMON_FN_NAMESPACE::ChangeCrateOrientation(index);
}

bool AlphaGhosts::ToggleSaveLoadBinds(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleSaveLoadBinds(enabled);
}

bool AlphaGhosts::ToggleUfoBind(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleUfoBind(enabled);
}

bool AlphaGhosts::RecordInput(bool enabled)
{
    return COMMON_FN_NAMESPACE::RecordInput(enabled);
}

bool AlphaGhosts::ToggleReplayInputBind(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleReplayInputBind(enabled);
}
