#include "pch.h"
#include "Core/Context.h"

namespace Context
{

int ClientNum = 0;

vec3 SavedPosition;

vec3 SavedAngles;

float CrateDistance = 150.0f;

float CrateHeight = 0.0f;

float CrateAngle = 0.0f;

_CratePositionPresets CratePositionPresets = CratePosition_BounceTest;

void *pBotEntity = nullptr;

void Reset()
{
    ClientNum = 0;
    SavedPosition = vec3();
    SavedAngles = vec3();
    pBotEntity = nullptr;
    CrateDistance = 150.0f;
    CrateHeight = 0.0f;
    CrateAngle = 0.0f;
    CratePositionPresets = CratePosition_BounceTest;
}

}
