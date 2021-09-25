#pragma once

#include "Core\Menu.h"
#include "Core\Option.h"


namespace MW2MenuFunctions
{

    VOID GoToSection(Menu* pMenu, Option* pContext);

    VOID Option1Callback(Menu*, Option*);

    VOID Option2Callback(Menu*, Option*);

}
