#pragma once

#include "Core\Menu.h"
#include "Games\MW2\GameFunctions.h"


namespace MW2MenuFunctions
{

VOID ToggleGodMode(Menu* pMenu);

VOID ToggleFallDamage(Menu* pMenu);

VOID ToggleAmmo(Menu* pMenu);

VOID ToggleElevators(Menu* pMenu);

VOID SpawnCP(Menu* pMenu);

VOID Knockback(Menu* pMenu);

VOID ToggleSaveLoadBinds(Menu* pMenu);

VOID SavePosition(Menu* pMenu);

VOID LoadPosition(Menu* pMenu);

VOID ToggleUFO(Menu* pMenu);

VOID SpawnBot(Menu* pMenu);

VOID TeleportBotToMe(Menu* pMenu);

VOID ToggleBotMovement(Menu* pMenu);

}