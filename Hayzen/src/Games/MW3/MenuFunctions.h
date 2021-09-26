#pragma once

#include "Core\Menu.h"
#include "Games\MW3\GameFunctions.h"


namespace MW3MenuFunctions
{

VOID ToggleGodMode(Menu* pMenu);

VOID ToggleFallDamage(Menu* pMenu);

VOID ToggleAmmo(Menu* pMenu);

VOID SpawnCP(Menu* pMenu);

VOID Knockback(Menu*);

VOID ToggleSaveLoadBinds(Menu* pMenu);

VOID SavePosition(Menu* pMenu);

VOID LoadPosition(Menu* pMenu);

VOID ToggleUFO(Menu* pMenu);

}
