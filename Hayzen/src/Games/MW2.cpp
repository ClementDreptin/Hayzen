#include "pch.h"
#include "MW2.h"

#include "Utils\Xbox.h"

//Detour xamInputGetStateDetour;
//Detour xamLoaderTerminateTitleDetour;

void MW2::Init()
{
	Xbox::XNotify("Hayzen - MW2 Multiplayer Detected!");

	//xamLoaderTerminateTitleDetour.DetourFunction((void*)s_XamLoaderTerminateTitleAddr, XamLoaderTerminateTitle_Hook, false);
}

/*void MW2::XamLoaderTerminateTitle_Hook()
{
	DbgPrint("XamLoaderTerminateTitle called\n");
	//xamLoaderTerminateTitleDetour.RestoreFunction();
}*/

/*DWORD MW2::XamInputGetState_Hook(DWORD userIndex, DWORD flags, PXINPUT_STATE pState)
{
	//auto Original = xamInputGetStateDetour.GetOriginal<decltype(&XamInputGetState_Hook)>();
	//return Original(userIndex, flags, pState);
	return 0;
}

void MW2::SCR_DrawScreenField_Hook(const int localClientNum, int refreshedUI)
{

}*/