#include "pch.h"
#include "Games/MW2/MenuFunctions.h"

#include "Games/MW2/GameFunctions.h"
#include "Games/MW2/MW2Title.h"

using namespace MW2::Game;

#ifdef COMMON_FN_NAMESPACE
    #undef COMMON_FN_NAMESPACE
#endif
#define COMMON_FN_NAMESPACE MW2Common
#ifdef COMMON_TITLE
    #undef COMMON_TITLE
#endif
#define COMMON_TITLE MW2Title
#include "Games/Common/CommonFunctions.h"

#define GAME_MW2
#include "Games/Common/MultiplayerFunctions.h"
#undef GAME_MW2

bool MW2::ToggleGodMode(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleGodModeMP(enabled);
}

bool MW2::ToggleFallDamage(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleFallDamage(enabled, 0x82019C48);
}

bool MW2::ToggleAmmo(bool enabled)
{
    COMMON_FN_NAMESPACE::ToggleAmmoOptions options = {};
    options.Enabled = enabled;
    options.PatchAddress = 0x820E1724;
    options.DefaultValue = 0x7D1D4850;
    options.PatchValue = 0x7D284B78;

    return COMMON_FN_NAMESPACE::ToggleAmmo(options);
}

bool MW2::GoThroughInvisibleBarriers(bool enabled)
{
    COMMON_FN_NAMESPACE::GoThroughInvisibleBarriersOptions options = {};
    options.Enabled = enabled;
    options.PM_CheckLadderMoveAddress = 0x820D8D38;
    options.PmoveSingleAddress = 0x820D94F8;

    return COMMON_FN_NAMESPACE::GoThroughInvisibleBarriers(options);
}

void MW2::SpawnCrate()
{
    COMMON_FN_NAMESPACE::SpawnCrate();
}

void MW2::SpawnBlocker()
{
    COMMON_FN_NAMESPACE::SpawnBlocker();
}

bool MW2::ChangeCratePositionPresets(size_t index)
{
    return COMMON_FN_NAMESPACE::ChangeCratePositionPresets(index);
}

bool MW2::ChangeCrateOrientation(size_t index)
{
    return COMMON_FN_NAMESPACE::ChangeCrateOrientation(index);
}

bool MW2::ToggleSaveLoadBinds(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleSaveLoadBinds(enabled);
}

bool MW2::ToggleUfoBind(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleUfoBind(enabled);
}

void MW2::SpawnBot()
{
    COMMON_FN_NAMESPACE::SpawnBotOptions *pOptions = new COMMON_FN_NAMESPACE::SpawnBotOptions();
    pOptions->ServerIdAddress = 0x8360922C;
    pOptions->ClientsBaseAddress = 0x83620380 + 0x3818; // svs + offsetof(serverStatic_t, clients) = 0x83623B98

    COMMON_FN_NAMESPACE::SpawnBot(pOptions);
}

void MW2::TeleportBotToMe()
{
    COMMON_FN_NAMESPACE::TeleportBotToMe();
}

bool MW2::ToggleBotMovement(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleBotMovement(enabled);
}

bool MW2::ToggleBotAttack(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleBotAttack(enabled);
}

bool MW2::RecordInput(bool enabled)
{
    return COMMON_FN_NAMESPACE::RecordInput(enabled);
}

bool MW2::ToggleReplayInputBind(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleReplayInputBind(enabled);
}
