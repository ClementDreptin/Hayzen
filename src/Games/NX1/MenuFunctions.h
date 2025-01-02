#pragma once

namespace NX1
{

bool ToggleGodMode(void *pParameters);

bool GoThroughInvisibleBarriers(void *pParameters);

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
