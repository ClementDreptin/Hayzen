#include "pch.h"
#include "Games/SpecOps/AlphaMW2/MenuFunctions.h"

#include "Games/SpecOps/AlphaMW2/GameFunctions.h"
#include "Games/SpecOps/AlphaMW2/SpecOpsAlphaMW2Title.h"

using namespace SpecOpsAlphaMW2::Game;

#ifdef COMMON_FN_NAMESPACE
    #undef COMMON_FN_NAMESPACE
#endif
#define COMMON_FN_NAMESPACE SpecOpsAlphaMW2Common
#ifdef COMMON_TITLE
    #undef COMMON_TITLE
#endif
#define COMMON_TITLE SpecOpsAlphaMW2Title
#include "Games/Common/CommonFunctions.h"

#define GAME_ALPHAMW2
#include "Games/Common/SpecOpsFunctions.h"
#undef GAME_ALPHAMW2

bool SpecOpsAlphaMW2::ToggleGodMode(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleGodModeSP(pParameters);
}

bool SpecOpsAlphaMW2::ToggleAmmo(void *pParameters)
{
    XASSERT(pParameters != nullptr);

    bool enabled = *reinterpret_cast<bool *>(pParameters);

    COMMON_FN_NAMESPACE::ToggleAmmoOptions options = {};
    options.Enabled = enabled;
    options.PatchAddress = 0x82328610;
    options.DefaultValue = 0x7D1D4850;
    options.PatchValue = 0x7D284B78;

    return COMMON_FN_NAMESPACE::ToggleAmmo(options);
}

bool SpecOpsAlphaMW2::ChangeJumpHeight(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ChangeJumpHeight(pParameters);
}

bool SpecOpsAlphaMW2::GoThroughInvisibleBarriers(void *pParameters)
{
    XASSERT(pParameters != nullptr);

    bool enabled = *reinterpret_cast<bool *>(pParameters);

    COMMON_FN_NAMESPACE::GoThroughInvisibleBarriersOptions options = {};
    options.Enabled = enabled;
    options.PM_CheckLadderMoveAddress = 0x8231FCE0;
    options.PmoveSingleAddress = 0x823204B0;

    return COMMON_FN_NAMESPACE::GoThroughInvisibleBarriers(options);
}

bool SpecOpsAlphaMW2::RecordInput(void *pParameters)
{
    return COMMON_FN_NAMESPACE::RecordInput(pParameters);
}

bool SpecOpsAlphaMW2::ToggleReplayInputBind(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleReplayInputBind(pParameters);
}

bool SpecOpsAlphaMW2::ToggleSaveLoadBinds(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleSaveLoadBinds(pParameters);
}

bool SpecOpsAlphaMW2::ToggleUfoBind(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleUfoBind(pParameters);
}

bool SpecOpsAlphaMW2::ToggleSecondPlayerGodMode(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleSecondPlayerGodMode(pParameters);
}

bool SpecOpsAlphaMW2::TeleportSecondPlayerToMe(void *)
{
    return COMMON_FN_NAMESPACE::TeleportSecondPlayerToMe();
}
