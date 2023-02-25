#include "pch.h"
#include "Core/Context.h"

namespace Context
{

int ClientNum = 0;

bool BindsEnabled = false;

vec3 SavedPosition;

vec3 SavedAngles;

Option::Callback SavePositionFn = nullptr;

Option::Callback LoadPositionFn = nullptr;

void *pBotEntity = nullptr;

bool DisplayControlsText = true;

void Reset()
{
    ClientNum = 0;
    BindsEnabled = false;
    SavedPosition = vec3();
    SavedAngles = vec3();
    SavePositionFn = nullptr;
    LoadPositionFn = nullptr;
    pBotEntity = nullptr;
}

}
