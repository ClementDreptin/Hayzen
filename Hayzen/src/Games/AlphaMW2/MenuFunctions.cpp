#include "pch.h"
#include "Games\AlphaMW2\MenuFunctions.h"

using namespace AlphaMW2::Game;

#ifdef COMMON_FN_NAMESPACE
#undef COMMON_FN_NAMESPACE 
#endif
#define COMMON_FN_NAMESPACE AlphaMW2Common

#include "Games\Common\CommonFunctions.h"
#include "Games\Common\MultiplayerFunctions.h"


void AlphaMW2::ToggleGodMode(Menu *pMenu) { COMMON_FN_NAMESPACE::ToggleGodModeMP(pMenu); }

void AlphaMW2::ToggleFallDamage(Menu *pMenu)
{
    int iClientNum = pMenu->GetClientNum();

    if (Dvar_GetFloat("bg_fallDamageMinHeight") == 128.0f)
    {
        SetClientDvar(-1, "bg_fallDamageMinHeight", "998");
        SetClientDvar(-1, "bg_fallDamageMaxHeight", "999");
        iPrintLn(iClientNum, "Fall Damage ^2Off");
    }
    else
    {
        SetClientDvar(-1, "bg_fallDamageMinHeight", "128");
        SetClientDvar(-1, "bg_fallDamageMaxHeight", "300");
        iPrintLn(iClientNum, "Fall Damage ^1On");
    }
}

void AlphaMW2::ToggleAmmo(Menu *pMenu)
{
    int iClientNum = pMenu->GetClientNum();

    if (!Dvar_GetBool("player_sustainAmmo"))
    {
        SetClientDvar(-1, "player_sustainAmmo", "1");
        iPrintLn(iClientNum, "Unlimited Ammo ^2On");
    }
    else
    {
        SetClientDvar(-1, "player_sustainAmmo", "0");
        iPrintLn(iClientNum, "Unlimited Ammo ^1Off");
    }
}

void AlphaMW2::SpawnCarePackage(Menu *pMenu) {COMMON_FN_NAMESPACE::SpawnCarePackage(pMenu); }

void AlphaMW2::ToggleSaveLoadBinds(Menu *pMenu) { COMMON_FN_NAMESPACE::ToggleSaveLoadBinds(pMenu); }

void AlphaMW2::SavePosition(Menu *pMenu) { COMMON_FN_NAMESPACE::SavePosition(pMenu); }

void AlphaMW2::LoadPosition(Menu *pMenu) { COMMON_FN_NAMESPACE::LoadPosition(pMenu); }

void AlphaMW2::ToggleUfo(Menu *pMenu) { COMMON_FN_NAMESPACE::ToggleUfo(pMenu); }

void AlphaMW2::SpawnBot(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::SpawnBotOptions *pOptions = new COMMON_FN_NAMESPACE::SpawnBotOptions();
    pOptions->pMenu = pMenu;
    pOptions->dwServerIdAddress = 0x8355D5C4;
    pOptions->dwClientsBaseAddress = 0x83577D98;

    COMMON_FN_NAMESPACE::SpawnBot(pOptions);
}

void AlphaMW2::TeleportBotToMe(Menu *pMenu) { COMMON_FN_NAMESPACE::TeleportBotToMe(pMenu); }

void AlphaMW2::ToggleBotMovement(Menu *pMenu) { COMMON_FN_NAMESPACE::ToggleBotMovement(pMenu); }
