#pragma once

#include "Core\Menu.h"
#include "Games\SpecOps\MW3\GameFunctions.h"


namespace SpecOpsMW3MenuFunctions
{

// Toggle God Mode.
VOID ToggleGodMode(Menu* pMenu);

// Toggle unlimited ammo.
VOID ToggleAmmo(Menu* pMenu);

// Toggle save and load binds.
VOID ToggleSaveLoadBinds(Menu* pMenu);

// Save the current player's position.
VOID SavePosition(Menu* pMenu);

// Load the previously saved player's position.
VOID LoadPosition(Menu* pMenu);

// Toggle UFO.
VOID ToggleUFO(Menu* pMenu);

// Toggle God Mode for the second player.
VOID ToggleSecondPlayerGodMode(Menu* pMenu);

// Teleport the second player in front of the first player.
VOID TeleportSecondPlayerToMe(Menu* pMenu);

}
