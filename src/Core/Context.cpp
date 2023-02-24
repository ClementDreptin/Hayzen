#include "pch.h"
#include "Core/Context.h"

namespace Context
{

int ClientNum = 0;

vec3 SavedPosition;

vec3 SavedAngles;

void Reset()
{
    ClientNum = 0;
    SavedPosition = vec3();
    SavedAngles = vec3();
}

}
