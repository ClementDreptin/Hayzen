#pragma once

namespace SpecOpsMW3
{

bool ToggleGodMode(void *pParameters);

bool ToggleAmmo(void *pParameters);

bool ChangeJumpHeight(void *pParameters);

bool GoThroughInvisibleBarriers(void *pParameters);

bool RecordInput(void *pParameters);

bool ToggleReplayInputBind(void *pParameters);

bool ToggleSaveLoadBinds(void *pParameters);

bool SavePosition(void *);

bool LoadPosition(void *);

bool ToggleUfo(void *pParameters);

bool ToggleSecondPlayerGodMode(void *pParameters);

bool TeleportSecondPlayerToMe(void *);

}
