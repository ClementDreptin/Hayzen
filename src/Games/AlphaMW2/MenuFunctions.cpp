#include "pch.h"
#include "Games/AlphaMW2/MenuFunctions.h"

#include "Games/AlphaMW2/GameFunctions.h"

using namespace AlphaMW2::Game;

#ifdef COMMON_FN_NAMESPACE
    #undef COMMON_FN_NAMESPACE
#endif
#define COMMON_FN_NAMESPACE AlphaMW2Common
#include "Games/Common/CommonFunctions.h"

#define GAME_ALPHAMW2
#include "Games/Common/MultiplayerFunctions.h"
#undef GAME_ALPHAMW2

bool AlphaMW2::ToggleGodMode(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleGodModeMP(pParameters);
}

bool AlphaMW2::ToggleFallDamage(void *pParameters)
{
    // For the MW2 Alpha we can't use the common function because changing the constant value
    // doesn't work so we went back to the old dvar way

    bool enabled = *reinterpret_cast<bool *>(pParameters);

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

bool AlphaMW2::ToggleAmmo(void *pParameters)
{
    bool enabled = *reinterpret_cast<bool *>(pParameters);

    COMMON_FN_NAMESPACE::ToggleAmmoOptions options = { 0 };
    options.Enabled = enabled;
    options.PatchAddress = 0x82113628;
    options.DefaultValue = 0x7D1E4850;
    options.PatchValue = 0x7D284B78;

    return COMMON_FN_NAMESPACE::ToggleAmmo(options);
}

bool AlphaMW2::SpawnCarePackage(void *)
{
    return COMMON_FN_NAMESPACE::SpawnCarePackage();
}

bool AlphaMW2::ChangeJumpHeight(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ChangeJumpHeight(pParameters);
}

bool AlphaMW2::ToggleSaveLoadBinds(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleSaveLoadBinds(pParameters);
}

bool AlphaMW2::SavePosition(void *)
{
    return COMMON_FN_NAMESPACE::SavePosition();
}

bool AlphaMW2::LoadPosition(void *)
{
    return COMMON_FN_NAMESPACE::LoadPosition();
}

bool AlphaMW2::ToggleUfo(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleUfo(pParameters);
}

bool AlphaMW2::SpawnBot(void *)
{
    COMMON_FN_NAMESPACE::SpawnBotOptions *pOptions = new COMMON_FN_NAMESPACE::SpawnBotOptions();
    pOptions->ServerIdAddress = 0x8355D5C4;
    pOptions->ClientsBaseAddress = 0x83577D98;

    return COMMON_FN_NAMESPACE::SpawnBot(pOptions);
}

bool AlphaMW2::TeleportBotToMe(void *)
{
    return COMMON_FN_NAMESPACE::TeleportBotToMe();
}

bool AlphaMW2::ToggleBotMovement(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleBotMovement(pParameters);
}
