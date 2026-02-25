#include "pch.h"
#include "Games/AlphaMW2/MenuFunctions.h"

#include "Games/AlphaMW2/AlphaMW2Title.h"
#include "Games/AlphaMW2/GameFunctions.h"

using namespace AlphaMW2::Game;

#ifdef COMMON_FN_NAMESPACE
    #undef COMMON_FN_NAMESPACE
#endif
#define COMMON_FN_NAMESPACE AlphaMW2Common
#ifdef COMMON_TITLE
    #undef COMMON_TITLE
#endif
#define COMMON_TITLE AlphaMW2Title
#include "Games/Common/CommonFunctions.h"

#define GAME_ALPHAMW2
#include "Games/Common/MultiplayerFunctions.h"
#undef GAME_ALPHAMW2

bool AlphaMW2::ToggleGodMode(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleGodModeMP(enabled);
}

bool AlphaMW2::ToggleFallDamage(bool enabled)
{
    // For the MW2 Alpha we can't use the common function because changing the constant value
    // doesn't work so we went back to the old dvar way

    if (enabled)
    {
        SetClientDvar(-1, "bg_fallDamageMinHeight", "9998");
        SetClientDvar(-1, "bg_fallDamageMaxHeight", "9999");
    }
    else
    {
        SetClientDvar(-1, "bg_fallDamageMinHeight", "128");
        SetClientDvar(-1, "bg_fallDamageMaxHeight", "300");
    }

    return true;
}

bool AlphaMW2::ToggleAmmo(bool enabled)
{
    COMMON_FN_NAMESPACE::ToggleAmmoOptions options = {};
    options.Enabled = enabled;
    options.PatchAddress = 0x82113628;
    options.DefaultValue = 0x7D1E4850;
    options.PatchValue = 0x7D284B78;

    return COMMON_FN_NAMESPACE::ToggleAmmo(options);
}

bool AlphaMW2::ChangeJumpHeight(uint32_t value)
{
    // For the MW2 Alpha changing the constant dvar value doesn't work
    // so we went back to the old dvar way

    SetClientDvar(-1, "jump_height", std::to_string(static_cast<uint64_t>(value)));

    return true;
}

bool AlphaMW2::GoThroughInvisibleBarriers(bool enabled)
{
    COMMON_FN_NAMESPACE::GoThroughInvisibleBarriersOptions options = {};
    options.Enabled = enabled;
    options.PM_CheckLadderMoveAddress = 0x8210AD80;
    options.PmoveSingleAddress = 0x8210B540;

    return COMMON_FN_NAMESPACE::GoThroughInvisibleBarriers(options);
}

void AlphaMW2::SpawnCrate()
{
    COMMON_FN_NAMESPACE::SpawnCrate();
}

void AlphaMW2::SpawnBlocker()
{
    COMMON_FN_NAMESPACE::SpawnBlocker();
}

bool AlphaMW2::ChangeCratePositionPresets(size_t index)
{
    return COMMON_FN_NAMESPACE::ChangeCratePositionPresets(index);
}

bool AlphaMW2::ChangeCrateOrientation(size_t index)
{
    return COMMON_FN_NAMESPACE::ChangeCrateOrientation(index);
}

bool AlphaMW2::ToggleSaveLoadBinds(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleSaveLoadBinds(enabled);
}

bool AlphaMW2::ToggleUfoBind(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleUfoBind(enabled);
}

void AlphaMW2::SpawnBot()
{
    COMMON_FN_NAMESPACE::SpawnBotOptions *pOptions = new COMMON_FN_NAMESPACE::SpawnBotOptions();
    pOptions->ServerIdAddress = 0x8355D5C4;
    pOptions->ClientsBaseAddress = 0x83574580 + 0x3818; // svs + offsetof(serverStatic_t, clients) = 0x83577D98

    COMMON_FN_NAMESPACE::SpawnBot(pOptions);
}

void AlphaMW2::TeleportBotToMe()
{
    COMMON_FN_NAMESPACE::TeleportBotToMe();
}

bool AlphaMW2::ToggleBotMovement(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleBotMovement(enabled);
}

bool AlphaMW2::ToggleBotAttack(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleBotAttack(enabled);
}

bool AlphaMW2::RecordInput(bool enabled)
{
    return COMMON_FN_NAMESPACE::RecordInput(enabled);
}

bool AlphaMW2::ToggleReplayInputBind(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleReplayInputBind(enabled);
}
