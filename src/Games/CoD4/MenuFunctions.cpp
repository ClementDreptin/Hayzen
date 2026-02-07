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

bool CoD4::ToggleGodMode(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleGodModeMP(pParameters);
}

bool CoD4::ToggleFallDamage(void *pParameters)
{
    XASSERT(pParameters != nullptr);

    bool enabled = *reinterpret_cast<bool *>(pParameters);

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

bool CoD4::ToggleAmmo(void *pParameters)
{
    XASSERT(pParameters != nullptr);

    bool enabled = *reinterpret_cast<bool *>(pParameters);

    COMMON_FN_NAMESPACE::ToggleAmmoOptions options = {};
    options.Enabled = enabled;
    options.PatchAddress = 0x82332088;
    options.DefaultValue = 0x7D695850;
    options.PatchValue = 0x60000000;

    return COMMON_FN_NAMESPACE::ToggleAmmo(options);
}

bool CoD4::ChangeJumpHeight(void *pParameters)
{
    // For CoD4 changing the constant dvar value doesn't work
    // so we went back to the old dvar way

    XASSERT(pParameters != nullptr);

    uint32_t value = *reinterpret_cast<uint32_t *>(pParameters);

    SetClientDvar(0, "jump_height", std::to_string(static_cast<uint64_t>(value)));

    return true;
}

bool CoD4::GoThroughInvisibleBarriers(void *pParameters)
{
    XASSERT(pParameters != nullptr);

    bool enabled = *reinterpret_cast<bool *>(pParameters);

    COMMON_FN_NAMESPACE::GoThroughInvisibleBarriersOptions options = {};
    options.Enabled = enabled;
    options.PM_CheckLadderMoveAddress = 0x823354D0;
    options.PmoveSingleAddress = 0x8233A938;

    return COMMON_FN_NAMESPACE::GoThroughInvisibleBarriers(options);
}

bool CoD4::RecordInput(void *pParameters)
{
    return COMMON_FN_NAMESPACE::RecordInput(pParameters);
}

bool CoD4::ToggleReplayInputBind(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleReplayInputBind(pParameters);
}

bool CoD4::SpawnCarePackage(void *)
{
    return COMMON_FN_NAMESPACE::SpawnCarePackage();
}

bool CoD4::SpawnBlocker(void *)
{
    // TODO: fix orientation

    return COMMON_FN_NAMESPACE::SpawnBlocker();
}

bool CoD4::ChangeCarePackagePositionPresets(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ChangeCarePackagePositionPresets(pParameters);
}

bool CoD4::ChangeCarePackageOrientation(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ChangeCarePackageOrientation(pParameters);
}

bool CoD4::ToggleSaveLoadBinds(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleSaveLoadBinds(pParameters);
}

bool CoD4::ToggleUfoBind(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleUfoBind(pParameters);
}

bool CoD4::SpawnBot(void *)
{
    COMMON_FN_NAMESPACE::SpawnBotOptions *pOptions = new COMMON_FN_NAMESPACE::SpawnBotOptions();
    pOptions->ServerIdAddress = 0x85027538;
    pOptions->ClientsBaseAddress = 0x82EE3500 + 0xA100C; // svs + offsetof(serverStatic_t, clients) = 0x82F8450C

    return COMMON_FN_NAMESPACE::SpawnBot(pOptions);
}

bool CoD4::TeleportBotToMe(void *)
{
    return COMMON_FN_NAMESPACE::TeleportBotToMe();
}

bool CoD4::ToggleBotMovement(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleBotMovement(pParameters);
}
