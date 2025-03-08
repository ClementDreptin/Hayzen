#pragma once

namespace MW2
{

bool ToggleGodMode(void *pParameters);

bool ToggleFallDamage(void *pParameters);

bool ToggleAmmo(void *pParameters);

bool ToggleElevators(void *pParameters);

bool Knockback(void *pParameters);

bool GoThroughInvisibleBarriers(void *pParameters);

bool RecordInput(void *pParameters);

bool ToggleReplayInputBind(void *pParameters);

bool SpawnCarePackage(void *);

bool SpawnBlocker(void *);

bool ChangeCarePackagePositionPresets(void *pParameters);

bool ChangeCarePackageOrientation(void *pParameters);

bool ToggleSaveLoadBinds(void *pParameters);

bool SavePosition(void *);

bool LoadPosition(void *);

bool ToggleUfo(void *pParameters);

bool SpawnBot(void *);

bool TeleportBotToMe(void *);

bool ToggleBotMovement(void *pParameters);

}
