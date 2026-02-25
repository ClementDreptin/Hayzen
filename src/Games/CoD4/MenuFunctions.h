#pragma once

namespace CoD4
{

bool ToggleGodMode(bool enabled);

bool ToggleFallDamage(bool enabled);

bool ToggleAmmo(bool enabled);

bool ChangeJumpHeight(uint32_t value);

bool GoThroughInvisibleBarriers(bool enabled);

void SpawnCrate();

void SpawnBlocker();

bool ChangeCratePositionPresets(size_t index);

bool ChangeCrateOrientation(size_t index);

bool ToggleSaveLoadBinds(bool enabled);

bool ToggleUfoBind(bool enabled);

void SpawnBot();

void TeleportBotToMe();

bool ToggleBotMovement(bool enabled);

bool RecordInput(bool enabled);

bool ToggleReplayInputBind(bool enabled);

}
