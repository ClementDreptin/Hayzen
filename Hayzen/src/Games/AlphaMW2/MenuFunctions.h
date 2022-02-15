#pragma once

#include "Core\Menu.h"
#include "Games\AlphaMW2\GameFunctions.h"


namespace AlphaMW2
{

void ToggleGodMode(Menu *pMenu);

void ToggleFallDamage(Menu *pMenu);

void ToggleAmmo(Menu *pMenu);

void SpawnCarePackage(Menu *pMenu);

void ToggleSaveLoadBinds(Menu *pMenu);

void SavePosition(Menu *pMenu);

void LoadPosition(Menu *pMenu);

void ToggleUfo(Menu *pMenu);

void SpawnBot(Menu *pMenu);

void TeleportBotToMe(Menu *pMenu);

void ToggleBotMovement(Menu *pMenu);

}
