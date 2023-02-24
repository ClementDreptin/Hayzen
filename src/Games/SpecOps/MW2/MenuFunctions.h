#pragma once

#include "Core/Menu.h"
#include "Games/SpecOps/MW2/GameFunctions.h"

namespace SpecOpsMW2
{

bool ToggleGodMode(void *pParameters);

bool ToggleAmmo(void *pParameters);

bool ChangeJumpHeight(void *pParameters);

bool ToggleSaveLoadBinds(void *);

bool SavePosition(void *);

bool LoadPosition(void *);

bool ToggleUfo(void *pParameters);

bool ToggleSecondPlayerGodMode(void *pParameters);

bool TeleportSecondPlayerToMe(void *);

}
