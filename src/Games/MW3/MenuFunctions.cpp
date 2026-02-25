#include "pch.h"
#include "Games/MW3/MenuFunctions.h"

#include "Games/MW3/GameFunctions.h"
#include "Games/MW3/MW3Title.h"

using namespace MW3::Game;

#ifdef COMMON_FN_NAMESPACE
    #undef COMMON_FN_NAMESPACE
#endif
#define COMMON_FN_NAMESPACE MW3Common
#ifdef COMMON_TITLE
    #undef COMMON_TITLE
#endif
#define COMMON_TITLE MW3Title
#include "Games/Common/CommonFunctions.h"

#define GAME_MW3
#include "Games/Common/MultiplayerFunctions.h"
#undef GAME_MW3

bool MW3::ToggleGodMode(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleGodModeMP(enabled);
}

bool MW3::ToggleFallDamage(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleFallDamage(enabled, 0x82000C04);
}

bool MW3::ToggleAmmo(bool enabled)
{
    COMMON_FN_NAMESPACE::ToggleAmmoOptions options = {};
    options.Enabled = enabled;
    options.PatchAddress = 0x820F63E4;
    options.DefaultValue = 0x7D3D5050;
    options.PatchValue = 0x7D495378;

    return COMMON_FN_NAMESPACE::ToggleAmmo(options);
}

bool MW3::GoThroughInvisibleBarriers(bool enabled)
{
    COMMON_FN_NAMESPACE::GoThroughInvisibleBarriersOptions options = {};
    options.Enabled = enabled;
    options.PM_CheckLadderMoveAddress = 0x820E94A0;
    options.PmoveSingleAddress = 0x820E9C68;

    return COMMON_FN_NAMESPACE::GoThroughInvisibleBarriers(options);
}

void MW3::SpawnCrate()
{
    COMMON_FN_NAMESPACE::SpawnCrate();
}

void MW3::SpawnBlocker()
{
    COMMON_FN_NAMESPACE::SpawnBlocker();
}

bool MW3::ChangeCratePositionPresets(size_t index)
{
    return COMMON_FN_NAMESPACE::ChangeCratePositionPresets(index);
}

bool MW3::ChangeCrateOrientation(size_t index)
{
    return COMMON_FN_NAMESPACE::ChangeCrateOrientation(index);
}

bool MW3::ToggleSaveLoadBinds(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleSaveLoadBinds(enabled);
}

bool MW3::ToggleUfoBind(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleUfoBind(enabled);
}

void MW3::SpawnBot()
{
    COMMON_FN_NAMESPACE::SpawnBotOptions *pOptions = new COMMON_FN_NAMESPACE::SpawnBotOptions();
    pOptions->ServerIdAddress = 0x8349746C;
    pOptions->ClientsBaseAddress = 0x834C0480 + 0x205E90; // svs + offsetof(serverStatic_t, clients) = 0x836C6310

    COMMON_FN_NAMESPACE::SpawnBot(pOptions);
}

void MW3::TeleportBotToMe()
{
    COMMON_FN_NAMESPACE::TeleportBotToMe();
}

bool MW3::ToggleBotMovement(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleBotMovement(enabled);
}

bool MW3::ToggleBotAttack(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleBotAttack(enabled);
}

bool MW3::RecordInput(bool enabled)
{
    return COMMON_FN_NAMESPACE::RecordInput(enabled);
}

bool MW3::ToggleReplayInputBind(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleReplayInputBind(enabled);
}
