#pragma once

#include "Core/Menu.h"
#include "Games/SpecOps/MW2/GameFunctions.h"

namespace SpecOpsMW2
{

void ToggleGodMode(void *pParameters);

void ToggleAmmo(void *pParameters);

void ChangeJumpHeight(void *pParameters);

void ToggleSaveLoadBinds(void *);

void SavePosition(void *);

void LoadPosition(void *);

void ToggleUfo(void *pParameters);
/*
void ToggleSecondPlayerGodMode(Menu *pMenu);

void TeleportSecondPlayerToMe(Menu *pMenu);
*/
}
