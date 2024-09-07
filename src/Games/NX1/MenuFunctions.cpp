#include "pch.h"
#include "Games/NX1/MenuFunctions.h"

#include "Games/NX1/GameFunctions.h"

using namespace NX1::Game;

#ifdef COMMON_FN_NAMESPACE
    #undef COMMON_FN_NAMESPACE
#endif
#define COMMON_FN_NAMESPACE NX1Common
#include "Games/Common/CommonFunctions.h"

#define GAME_NX1
#include "Games/Common/MultiplayerFunctions.h"
#undef GAME_NX1

bool NX1::ToggleGodMode(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleGodModeMP(pParameters);
}

bool NX1::ToggleSaveLoadBinds(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleSaveLoadBinds(pParameters);
}

bool NX1::SavePosition(void *)
{
    return COMMON_FN_NAMESPACE::SavePosition();
}

bool NX1::LoadPosition(void *)
{
    return COMMON_FN_NAMESPACE::LoadPosition();
}

bool NX1::ToggleUfo(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleUfo(pParameters);
}

bool NX1::SpawnCarePackage(void *)
{
    return COMMON_FN_NAMESPACE::SpawnCarePackage();
}

bool NX1::SpawnBot(void *pParameters)
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
