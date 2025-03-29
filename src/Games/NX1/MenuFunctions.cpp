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

bool NX1::ToggleGodMode(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleGodModeMP(pParameters);
}

bool NX1::GoThroughInvisibleBarriers(void *pParameters)
{
    XASSERT(pParameters != nullptr);

    bool enabled = *reinterpret_cast<bool *>(pParameters);

    COMMON_FN_NAMESPACE::GoThroughInvisibleBarriersOptions options = {};
    options.Enabled = enabled;
    options.PM_CheckLadderMoveAddress = 0x821111E8;
    options.PmoveSingleAddress = 0x82111970;

    return COMMON_FN_NAMESPACE::GoThroughInvisibleBarriers(options);
}

bool NX1::RecordInput(void *pParameters)
{
    return COMMON_FN_NAMESPACE::RecordInput(pParameters);
}

bool NX1::ToggleReplayInputBind(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleReplayInputBind(pParameters);
}

bool NX1::SpawnCarePackage(void *)
{
    return COMMON_FN_NAMESPACE::SpawnCarePackage();
}

bool NX1::SpawnBlocker(void *)
{
    return COMMON_FN_NAMESPACE::SpawnBlocker();
}

bool NX1::ChangeCarePackagePositionPresets(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ChangeCarePackagePositionPresets(pParameters);
}

bool NX1::ChangeCarePackageOrientation(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ChangeCarePackageOrientation(pParameters);
}

bool NX1::ToggleSaveLoadBinds(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleSaveLoadBinds(pParameters);
}

bool NX1::ToggleUfoBind(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleUfoBind(pParameters);
}

bool NX1::SpawnBot(void *)
{
    COMMON_FN_NAMESPACE::SpawnBotOptions *pOptions = new COMMON_FN_NAMESPACE::SpawnBotOptions();
    pOptions->ServerIdAddress = 0x83A6B78C;
    pOptions->ClientsBaseAddress = 0x83A82B00 + 0x381C;

    return COMMON_FN_NAMESPACE::SpawnBot(pOptions);
}

bool NX1::TeleportBotToMe(void *)
{
    return COMMON_FN_NAMESPACE::TeleportBotToMe();
}

bool NX1::ToggleBotMovement(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleBotMovement(pParameters);
}
