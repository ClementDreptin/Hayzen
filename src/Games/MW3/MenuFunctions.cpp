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

bool MW3::ToggleGodMode(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleGodModeMP(pParameters);
}

bool MW3::ToggleFallDamage(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleFallDamage(pParameters, 0x82000C04);
}

bool MW3::ToggleAmmo(void *pParameters)
{
    XASSERT(pParameters != nullptr);

    bool enabled = *reinterpret_cast<bool *>(pParameters);

    COMMON_FN_NAMESPACE::ToggleAmmoOptions options = {};
    options.Enabled = enabled;
    options.PatchAddress = 0x820F63E4;
    options.DefaultValue = 0x7D3D5050;
    options.PatchValue = 0x7D495378;

    return COMMON_FN_NAMESPACE::ToggleAmmo(options);
}

bool MW3::GoThroughInvisibleBarriers(void *pParameters)
{
    XASSERT(pParameters != nullptr);

    bool enabled = *reinterpret_cast<bool *>(pParameters);

    COMMON_FN_NAMESPACE::GoThroughInvisibleBarriersOptions options = {};
    options.Enabled = enabled;
    options.PM_CheckLadderMoveAddress = 0x820E94A0;
    options.PmoveSingleAddress = 0x820E9C68;

    return COMMON_FN_NAMESPACE::GoThroughInvisibleBarriers(options);
}

bool MW3::RecordInput(void *pParameters)
{
    return COMMON_FN_NAMESPACE::RecordInput(pParameters);
}

bool MW3::ToggleReplayInputBind(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleReplayInputBind(pParameters);
}

bool MW3::SpawnCarePackage(void *)
{
    return COMMON_FN_NAMESPACE::SpawnCarePackage();
}

bool MW3::SpawnBlocker(void *)
{
    return COMMON_FN_NAMESPACE::SpawnBlocker();
}

bool MW3::ChangeCarePackagePositionPresets(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ChangeCarePackagePositionPresets(pParameters);
}

bool MW3::ChangeCarePackageOrientation(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ChangeCarePackageOrientation(pParameters);
}

bool MW3::ToggleSaveLoadBinds(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleSaveLoadBinds(pParameters);
}

bool MW3::ToggleUfoBind(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleUfoBind(pParameters);
}

bool MW3::SpawnBot(void *)
{
    COMMON_FN_NAMESPACE::SpawnBotOptions *pOptions = new COMMON_FN_NAMESPACE::SpawnBotOptions();
    pOptions->ServerIdAddress = 0x8349746C;
    pOptions->ClientsBaseAddress = 0x834C0480 + 0x205E90; // svs + offsetof(serverStatic_t, clients) = 0x836C6310

    return COMMON_FN_NAMESPACE::SpawnBot(pOptions);
}

bool MW3::TeleportBotToMe(void *)
{
    return COMMON_FN_NAMESPACE::TeleportBotToMe();
}

bool MW3::ToggleBotMovement(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleBotMovement(pParameters);
}

bool MW3::ToggleBotAttack(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleBotAttack(pParameters);
}
