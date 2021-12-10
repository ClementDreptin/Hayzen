#pragma once

#include "Core\Menu.h"
#include "Games\MW2\GameFunctions.h"


namespace MW2MenuFunctions
{

// Toggle God Mode.
VOID ToggleGodMode(Menu *pMenu);

// Toggle fall damage.
VOID ToggleFallDamage(Menu *pMenu);

// Toggle unlimited ammo.
VOID ToggleAmmo(Menu *pMenu);

// Toggle elvators.
VOID ToggleElevators(Menu *pMenu);

// Spawn a care package.
VOID SpawnCP(Menu *pMenu);

// Open a keyboard to allow the user to change the knockback value.
VOID Knockback(Menu *pMenu);

// Toggle save and load binds.
VOID ToggleSaveLoadBinds(Menu *pMenu);

// Save the current player's position.
VOID SavePosition(Menu *pMenu);

// Load the previously saved player's position.
VOID LoadPosition(Menu *pMenu);

// Toggle UFO.
VOID ToggleUFO(Menu *pMenu);

// Spawn a bot.
VOID SpawnBot(Menu *pMenu);

// Teleport the bot in front of the player.
VOID TeleportBotToMe(Menu *pMenu);

// Toggle the bot's movement.
VOID ToggleBotMovement(Menu *pMenu);

}
