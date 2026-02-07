#pragma once

namespace AlphaMW2
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

bool SpawnBot(void *);

bool TeleportBotToMe(void *);

bool ToggleBotMovement(void *pParameters);

bool ToggleBotAttack(void *pParameters);

}
