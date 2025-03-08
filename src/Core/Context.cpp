#include "pch.h"
#include "Core/Context.h"

namespace Context
{

int ClientNum = 0;

vec3 SavedPosition;

vec3 SavedAngles;

void *pBotEntity = nullptr;

void Reset()
{
    ClientNum = 0;
    SavedPosition = vec3();
    SavedAngles = vec3();
    pBotEntity = nullptr;
}

}
