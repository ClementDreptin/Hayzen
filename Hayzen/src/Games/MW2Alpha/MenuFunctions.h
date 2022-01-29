#pragma once

#include "Core\Menu.h"
#include "Games\MW2Alpha\GameFunctions.h"


namespace AlphaMW2MenuFunctions
{

// Toggle God Mode.
void ToggleGodMode(Menu *pMenu);

// Toggle fall damage.
void ToggleFallDamage(Menu *pMenu);

// Toggle unlimited ammo.
void ToggleAmmo(Menu *pMenu);

// Spawn a care package.
void SpawnCP(Menu *pMenu);

// Toggle save and load binds.
void ToggleSaveLoadBinds(Menu *pMenu);

// Save the current player's position.
void SavePosition(Menu *pMenu);

// Load the previously saved player's position.
void LoadPosition(Menu *pMenu);

// Toggle UFO.
void ToggleUFO(Menu *pMenu);

// Spawn a bot.
void SpawnBot(Menu *pMenu);

// Teleport the bot in front of the player.
void TeleportBotToMe(Menu *pMenu);

// Toggle the bot's movement.
void ToggleBotMovement(Menu *pMenu);

}
