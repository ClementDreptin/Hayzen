#pragma once

#include "Core\Menu.h"
#include "Games\MW3\GameFunctions.h"


namespace MW3MenuFunctions
{

// Toggle God Mode.
VOID ToggleGodMode(Menu* pMenu);

// Toggle fall damage.
VOID ToggleFallDamage(Menu* pMenu);

// Toggle unlimited ammo.
VOID ToggleAmmo(Menu* pMenu);

// Spawn a care package.
VOID SpawnCP(Menu* pMenu);

// Toggle save and load binds.
VOID ToggleSaveLoadBinds(Menu* pMenu);

// Save the current player's position.
VOID SavePosition(Menu* pMenu);

// Load the previously saved player's position.
VOID LoadPosition(Menu* pMenu);

// Toggle UFO.
VOID ToggleUFO(Menu* pMenu);

}
