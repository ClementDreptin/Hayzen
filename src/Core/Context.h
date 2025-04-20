#pragma once

typedef enum _CarePackagePositionPresets
{
    CarePackagePosition_BounceTest,
    CarePackagePosition_MegaBounce,
    CarePackagePosition_Floor,
} CarePackagePositionPresets;

namespace Context
{

extern int ClientNum;

extern vec3 SavedPosition;

extern vec3 SavedAngles;

extern float CarePackageDistance;

extern float CarePackageHeight;

extern float CarePackageAngle;

extern _CarePackagePositionPresets CarePackagePositionPresets;

extern void *pBotEntity;

void Reset();

}
