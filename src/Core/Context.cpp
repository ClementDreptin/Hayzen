#include "pch.h"
#include "Core/Context.h"

namespace Context
{

int ClientNum = 0;

vec3 SavedPosition;

vec3 SavedAngles;

float CarePackageDistance = 150.0f;

float CarePackageHeight = 0.0f;

float CarePackageAngle = 0.0f;

_CarePackagePositionPresets CarePackagePositionPresets = CarePackagePosition_BounceTest;

void *pBotEntity = nullptr;

void Reset()
{
    ClientNum = 0;
    SavedPosition = vec3();
    SavedAngles = vec3();
    pBotEntity = nullptr;
    CarePackageDistance = 150.0f;
    CarePackageHeight = 0.0f;
    CarePackageAngle = 0.0f;
    CarePackagePositionPresets = CarePackagePosition_BounceTest;
}

}
