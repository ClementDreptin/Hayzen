#include "pch.h"
#include "Games/CoD4/MenuFunctions.h"

#include "Games/CoD4/GameFunctions.h"
#include "Games/CoD4/CoD4Title.h"

using namespace CoD4::Game;

#ifdef COMMON_FN_NAMESPACE
    #undef COMMON_FN_NAMESPACE
#endif
#define COMMON_FN_NAMESPACE CoD4Common
#ifdef COMMON_TITLE
    #undef COMMON_TITLE
#endif
#define COMMON_TITLE CoD4Title
#include "Games/Common/CommonFunctions.h"

#define GAME_COD4
#include "Games/Common/MultiplayerFunctions.h"
#undef GAME_COD4

bool CoD4::ToggleGodMode(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleGodModeMP(enabled);
}

bool CoD4::ToggleFallDamage(bool enabled)
{
    // For CoD4 we can't use the common function because changing the constant value
    // doesn't work so we went back to the old dvar way

    if (enabled)
    {
        SetClientDvar(0, "bg_fallDamageMinHeight", "9998");
        SetClientDvar(0, "bg_fallDamageMaxHeight", "9999");
    }
    else
    {
        SetClientDvar(0, "bg_fallDamageMinHeight", "128");
        SetClientDvar(0, "bg_fallDamageMaxHeight", "300");
    }

    return true;
}

bool CoD4::ToggleAmmo(bool enabled)
{
    COMMON_FN_NAMESPACE::ToggleAmmoOptions options = {};
    options.Enabled = enabled;
    options.PatchAddress = 0x82332088;
    options.DefaultValue = 0x7D695850;
    options.PatchValue = 0x60000000;

    return COMMON_FN_NAMESPACE::ToggleAmmo(options);
}

bool CoD4::ChangeJumpHeight(uint32_t value)
{
    // For CoD4 changing the constant dvar value doesn't work
    // so we went back to the old dvar way

    SetClientDvar(0, "jump_height", std::to_string(static_cast<uint64_t>(value)));

    return true;
}

bool CoD4::GoThroughInvisibleBarriers(bool enabled)
{
    COMMON_FN_NAMESPACE::GoThroughInvisibleBarriersOptions options = {};
    options.Enabled = enabled;
    options.PM_CheckLadderMoveAddress = 0x823354D0;
    options.PmoveSingleAddress = 0x8233A938;

    return COMMON_FN_NAMESPACE::GoThroughInvisibleBarriers(options);
}

void CoD4::SpawnCrate()
{
    COMMON_FN_NAMESPACE::SpawnCrate();
}

void CoD4::SpawnBlocker()
{
    // TODO: fix orientation

    COMMON_FN_NAMESPACE::SpawnBlocker();
}

bool CoD4::ChangeCratePositionPresets(size_t index)
{
    return COMMON_FN_NAMESPACE::ChangeCratePositionPresets(index);
}

bool CoD4::ChangeCrateOrientation(size_t index)
{
    return COMMON_FN_NAMESPACE::ChangeCrateOrientation(index);
}

bool CoD4::ToggleSaveLoadBinds(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleSaveLoadBinds(enabled);
}

bool CoD4::ToggleUfoBind(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleUfoBind(enabled);
}

void CoD4::SpawnBot()
{
    COMMON_FN_NAMESPACE::SpawnBotOptions *pOptions = new COMMON_FN_NAMESPACE::SpawnBotOptions();
    pOptions->ServerIdAddress = 0x85027538;
    pOptions->ClientsBaseAddress = 0x82EE3500 + 0xA100C; // svs + offsetof(serverStatic_t, clients) = 0x82F8450C

    COMMON_FN_NAMESPACE::SpawnBot(pOptions);
}

void CoD4::TeleportBotToMe()
{
    COMMON_FN_NAMESPACE::TeleportBotToMe();
}

bool CoD4::ToggleBotMovement(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleBotMovement(enabled);
}

bool CoD4::RecordInput(bool enabled)
{
    return COMMON_FN_NAMESPACE::RecordInput(enabled);
}

bool CoD4::ToggleReplayInputBind(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleReplayInputBind(enabled);
}
