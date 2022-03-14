#include "pch.h"
#include "Games\MW3\MenuFunctions.h"

using namespace MW3::Game;

#ifdef COMMON_FN_NAMESPACE
#undef COMMON_FN_NAMESPACE 
#endif
#define COMMON_FN_NAMESPACE MW3Common

#include "Games\Common\CommonFunctions.h"

#define COMMON_FN_NO_BOTS
#define CARE_PACKAGE_MODEL "com_plasticcase_trap_friendly"
#include "Games\Common\MultiplayerFunctions.h"
#undef CARE_PACKAGE_MODEL
#undef COMMON_FN_NO_BOTS


void MW3::ToggleGodMode(Menu *pMenu) { COMMON_FN_NAMESPACE::ToggleGodModeMP(pMenu); }

void MW3::ToggleFallDamage(Menu *pMenu) { COMMON_FN_NAMESPACE::ToggleFallDamage(pMenu, 0x82000C04); }

void MW3::ToggleAmmo(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::ToggleAmmoOptions Options;
    Options.pMenu = pMenu;
    Options.dwPatchAddress = 0x820F63E4;
    Options.dwDefaultValue = 0x7D3D5050;
    Options.dwPatchValue = 0x7D495378;

    COMMON_FN_NAMESPACE::ToggleAmmo(Options);
}

void MW3::SpawnCarePackage(Menu *pMenu) { COMMON_FN_NAMESPACE::SpawnCarePackage(pMenu); }

void MW3::ToggleSaveLoadBinds(Menu *pMenu) { COMMON_FN_NAMESPACE::ToggleSaveLoadBinds(pMenu); }

void MW3::SavePosition(Menu *pMenu) { COMMON_FN_NAMESPACE::SavePosition(pMenu); }

void MW3::LoadPosition(Menu *pMenu) { COMMON_FN_NAMESPACE::LoadPosition(pMenu); }

void MW3::ToggleUfo(Menu *pMenu) { COMMON_FN_NAMESPACE::ToggleUfo(pMenu); }
