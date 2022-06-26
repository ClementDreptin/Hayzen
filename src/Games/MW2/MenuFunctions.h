#pragma once

#include "Core\Menu.h"
#include "Games\MW2\GameFunctions.h"

namespace MW2
{

void ToggleGodMode(Menu *pMenu);

void ToggleFallDamage(Menu *pMenu);

void ToggleAmmo(Menu *pMenu);

// Toggle elvators.
void ToggleElevators(Menu *pMenu);

void SpawnCarePackage(Menu *pMenu);

// Open a keyboard to allow the user to change the knockback value.
void Knockback(Menu *pMenu);

void ToggleSaveLoadBinds(Menu *pMenu);

void SavePosition(Menu *pMenu);

void LoadPosition(Menu *pMenu);

void ToggleUfo(Menu *pMenu);

void SpawnBot(Menu *pMenu);

void TeleportBotToMe(Menu *pMenu);

void ToggleBotMovement(Menu *pMenu);

}
