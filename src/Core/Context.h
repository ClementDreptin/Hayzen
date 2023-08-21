#pragma once

#include "Options/Option.h"

namespace Context
{

extern int ClientNum;

extern bool BindsEnabled;

extern vec3 SavedPosition;

extern vec3 SavedAngles;

extern Option::Callback SavePositionFn;

extern Option::Callback LoadPositionFn;

extern void *pBotEntity;

void Reset();

}
