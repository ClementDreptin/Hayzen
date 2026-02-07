#pragma once

typedef enum _CratePositionPresets
{
    CratePosition_BounceTest,
    CratePosition_MegaBounce,
    CratePosition_Floor,
} CratePositionPresets;

namespace Context
{

extern int ClientNum;

extern vec3 SavedPosition;

extern vec3 SavedAngles;

extern float CrateDistance;

extern float CrateHeight;

extern float CrateAngle;

extern _CratePositionPresets CratePositionPresets;

extern void *pBotEntity;

void Reset();

}
