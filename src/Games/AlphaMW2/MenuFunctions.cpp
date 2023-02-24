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

void AlphaMW2::ToggleGodMode(void *pParameters)
{
    COMMON_FN_NAMESPACE::ToggleGodModeMP(pParameters);
}

void AlphaMW2::ToggleFallDamage(void *pParameters)
{
    // For the MW2 Alpha we can't use the common function because changing the constant value
    // doesn't work so we went back to the old dvar way

    bool enabled = *reinterpret_cast<bool *>(pParameters);

    if (enabled)
    {
        SetClientDvar(-1, "bg_fallDamageMinHeight", "998");
        SetClientDvar(-1, "bg_fallDamageMaxHeight", "999");
    }
    else
    {
        SetClientDvar(-1, "bg_fallDamageMinHeight", "128");
        SetClientDvar(-1, "bg_fallDamageMaxHeight", "300");
    }
}

void AlphaMW2::ToggleAmmo(void *pParameters)
{
    bool enabled = *reinterpret_cast<bool *>(pParameters);

    COMMON_FN_NAMESPACE::ToggleAmmoOptions options = { 0 };
    options.Enabled = enabled;
    options.PatchAddress = 0x82113628;
    options.DefaultValue = 0x7D1E4850;
    options.PatchValue = 0x7D284B78;

    COMMON_FN_NAMESPACE::ToggleAmmo(options);
}

void AlphaMW2::SpawnCarePackage(void *)
{
    COMMON_FN_NAMESPACE::SpawnCarePackage();
}

/* void AlphaMW2::ToggleSaveLoadBinds(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::ToggleSaveLoadBinds(pMenu);
} */

void AlphaMW2::SavePosition(void *)
{
    COMMON_FN_NAMESPACE::SavePosition();
}

void AlphaMW2::LoadPosition(void *)
{
    COMMON_FN_NAMESPACE::LoadPosition();
}

/* void AlphaMW2::ToggleUfo(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::ToggleUfo(pMenu);
}

void AlphaMW2::SpawnBot(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::SpawnBotOptions *pOptions = new COMMON_FN_NAMESPACE::SpawnBotOptions();
    pOptions->pMenu = pMenu;
    pOptions->serverIdAddress = 0x8355D5C4;
    pOptions->clientsBaseAddress = 0x83577D98;

    COMMON_FN_NAMESPACE::SpawnBot(pOptions);
}

void AlphaMW2::TeleportBotToMe(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::TeleportBotToMe(pMenu);
}

void AlphaMW2::ToggleBotMovement(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::ToggleBotMovement(pMenu);
} */
