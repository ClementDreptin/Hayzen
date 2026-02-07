#pragma once

namespace AlphaGhosts
{

bool ToggleGodMode(void *pParameters);

bool ToggleFallDamage(void *pParameters);

bool ToggleAmmo(void *pParameters);

bool ChangeJumpHeight(void *pParameters);

bool GoThroughInvisibleBarriers(void *pParameters);

bool RecordInput(void *pParameters);

bool ToggleReplayInputBind(void *pParameters);

bool SpawnCrate(void *);

bool SpawnBlocker(void *);

bool ChangeCratePositionPresets(void *pParameters);

bool ChangeCrateOrientation(void *pParameters);

bool ToggleSaveLoadBinds(void *pParameters);

bool ToggleUfoBind(void *pParameters);

}
