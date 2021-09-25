#include "pch.h"
#include "Games\MW2\MenuFunctions.h"


VOID MW2MenuFunctions::GoToSection(Menu* pMenu, Option* pContext)
{
    pMenu->GoToSection(pContext->GetText());
}


VOID MW2MenuFunctions::Option1Callback(Menu*, Option*)
{
    Log::Info("Option 1 clicked");
}


VOID MW2MenuFunctions::Option2Callback(Menu*, Option*)
{
    Log::Info("Option 2 clicked");
}
