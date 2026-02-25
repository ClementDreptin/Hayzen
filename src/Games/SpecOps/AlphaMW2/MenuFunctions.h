#pragma once

namespace SpecOpsAlphaMW2
{

bool ToggleGodMode(bool enabled);

bool ToggleAmmo(bool enabled);

bool ChangeJumpHeight(uint32_t value);

bool ToggleReplayInputBind(bool enabled);

bool ToggleSaveLoadBinds(bool enabled);

bool ToggleUfoBind(bool enabled);

bool ToggleSecondPlayerGodMode(bool enabled);

void TeleportSecondPlayerToMe();

bool GoThroughInvisibleBarriers(bool enabled);

bool RecordInput(bool enabled);

}
