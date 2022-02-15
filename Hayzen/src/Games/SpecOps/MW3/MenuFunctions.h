#pragma once

#include "Core\Menu.h"
#include "Games\SpecOps\MW3\GameFunctions.h"


namespace SpecOpsMW3
{

void ToggleGodMode(Menu *pMenu);

void ToggleAmmo(Menu *pMenu);

void ChangeJumpHeight(Menu *pMenu);

void ToggleSaveLoadBinds(Menu *pMenu);

void SavePosition(Menu *pMenu);

void LoadPosition(Menu *pMenu);

void ToggleUfo(Menu *pMenu);

void ToggleSecondPlayerGodMode(Menu *pMenu);

void TeleportSecondPlayerToMe(Menu *pMenu);

}
