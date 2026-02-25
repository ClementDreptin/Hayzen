#pragma once

namespace SpecOpsMW3
{

bool ToggleGodMode(bool enabled);

bool ToggleAmmo(bool enabled);

bool ChangeJumpHeight(uint32_t value);

bool GoThroughInvisibleBarriers(bool enabled);

bool ToggleSaveLoadBinds(bool enabled);

bool ToggleUfoBind(bool enabled);

bool ToggleSecondPlayerGodMode(bool enabled);

void TeleportSecondPlayerToMe();

bool RecordInput(bool enabled);

bool ToggleReplayInputBind(bool enabled);

}
