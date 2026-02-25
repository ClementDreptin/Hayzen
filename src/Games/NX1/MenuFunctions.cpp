#include "pch.h"
#include "Games/NX1/MenuFunctions.h"

#include "Games/NX1/GameFunctions.h"
#include "Games/NX1/NX1Title.h"

using namespace NX1::Game;

#ifdef COMMON_FN_NAMESPACE
    #undef COMMON_FN_NAMESPACE
#endif
#define COMMON_FN_NAMESPACE NX1Common
#ifdef COMMON_TITLE
    #undef COMMON_TITLE
#endif
#define COMMON_TITLE NX1Title
#include "Games/Common/CommonFunctions.h"

#define GAME_NX1
#include "Games/Common/MultiplayerFunctions.h"
#undef GAME_NX1

bool NX1::ToggleGodMode(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleGodModeMP(enabled);
}

bool NX1::ToggleFallDamage(bool enabled)
{
    // For NX1 we can't use the common function because changing the constant value
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

bool NX1::ToggleAmmo(bool enabled)
{
    COMMON_FN_NAMESPACE::ToggleAmmoOptions options = {};
    options.Enabled = enabled;
    options.PatchAddress = 0x8211C080;
    options.DefaultValue = 0x7D1E4850;
    options.PatchValue = 0x7D284B78;

    return COMMON_FN_NAMESPACE::ToggleAmmo(options);
}

bool NX1::ChangeJumpHeight(uint32_t value)
{
    // For NX1 changing the constant dvar value doesn't work
    // so we went back to the old dvar way

    SetClientDvar(-1, "jump_height", std::to_string(static_cast<uint64_t>(value)));

    return true;
}

bool NX1::GoThroughInvisibleBarriers(bool enabled)
{
    COMMON_FN_NAMESPACE::GoThroughInvisibleBarriersOptions options = {};
    options.Enabled = enabled;
    options.PM_CheckLadderMoveAddress = 0x821111E8;
    options.PmoveSingleAddress = 0x82111970;

    return COMMON_FN_NAMESPACE::GoThroughInvisibleBarriers(options);
}

void NX1::SpawnCrate()
{
    COMMON_FN_NAMESPACE::SpawnCrate();
}

void NX1::SpawnBlocker()
{
    COMMON_FN_NAMESPACE::SpawnBlocker();
}

bool NX1::ChangeCratePositionPresets(size_t index)
{
    return COMMON_FN_NAMESPACE::ChangeCratePositionPresets(index);
}

bool NX1::ChangeCrateOrientation(size_t index)
{
    return COMMON_FN_NAMESPACE::ChangeCrateOrientation(index);
}

bool NX1::ToggleSaveLoadBinds(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleSaveLoadBinds(enabled);
}

bool NX1::ToggleUfoBind(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleUfoBind(enabled);
}

void NX1::SpawnBot()
{
    COMMON_FN_NAMESPACE::SpawnBotOptions *pOptions = new COMMON_FN_NAMESPACE::SpawnBotOptions();
    pOptions->ServerIdAddress = 0x83A6B78C;
    pOptions->ClientsBaseAddress = 0x83A82B00 + 0x381C; // svs + offsetof(serverStatic_t, clients) = 0x83A8631C

    COMMON_FN_NAMESPACE::SpawnBot(pOptions);
}

void NX1::TeleportBotToMe()
{
    COMMON_FN_NAMESPACE::TeleportBotToMe();
}

bool NX1::ToggleBotMovement(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleBotMovement(enabled);
}

bool NX1::ToggleBotAttack(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleBotAttack(enabled);
}

bool NX1::RecordInput(bool enabled)
{
    return COMMON_FN_NAMESPACE::RecordInput(enabled);
}

bool NX1::ToggleReplayInputBind(bool enabled)
{
    return COMMON_FN_NAMESPACE::ToggleReplayInputBind(enabled);
}
