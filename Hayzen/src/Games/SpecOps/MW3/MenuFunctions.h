#pragma once

#include "Core\Menu.h"
#include "Games\SpecOps\MW3\GameFunctions.h"


namespace SpecOpsMW3
{

// Toggle God Mode.
void ToggleGodMode(Menu *pMenu);

// Toggle unlimited ammo.
void ToggleAmmo(Menu *pMenu);

// Change the jump height.
void ChangeJumpHeight(Menu *pMenu);

// Toggle save and load binds.
void ToggleSaveLoadBinds(Menu *pMenu);

// Save the current player's position.
void SavePosition(Menu *pMenu);

// Load the previously saved player's position.
void LoadPosition(Menu *pMenu);

// Toggle UFO.
void ToggleUfo(Menu *pMenu);

// Toggle God Mode for the second player.
void ToggleSecondPlayerGodMode(Menu *pMenu);

// Teleport the second player in front of the first player.
void TeleportSecondPlayerToMe(Menu *pMenu);

}
