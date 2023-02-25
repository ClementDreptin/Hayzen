#include "pch.h"
#include "Games/SpecOps/MW3/MenuFunctions.h"

#include "Games/SpecOps/MW3/GameFunctions.h"

using namespace SpecOpsMW3::Game;

#ifdef COMMON_FN_NAMESPACE
    #undef COMMON_FN_NAMESPACE
#endif
#define COMMON_FN_NAMESPACE SpecOpsMW3Common
#include "Games/Common/CommonFunctions.h"

#define GAME_MW3
#include "Games/Common/SpecOpsFunctions.h"
#undef GAME_MW3

bool SpecOpsMW3::ToggleGodMode(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleGodModeSP(pParameters);
}

bool SpecOpsMW3::ToggleAmmo(void *pParameters)
{
    bool enabled = *reinterpret_cast<bool *>(pParameters);

    COMMON_FN_NAMESPACE::ToggleAmmoOptions options = { 0 };
    options.Enabled = enabled;
    options.PatchAddress = 0x8235BB54;
    options.DefaultValue = 0x7D3D5050;
    options.PatchValue = 0x7D495378;

    return COMMON_FN_NAMESPACE::ToggleAmmo(options);
}

bool SpecOpsMW3::ChangeJumpHeight(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ChangeJumpHeight(pParameters);
}

bool SpecOpsMW3::ToggleSaveLoadBinds(void *)
{
    return COMMON_FN_NAMESPACE::ToggleSaveLoadBinds();
}

bool SpecOpsMW3::SavePosition(void *)
{
    return COMMON_FN_NAMESPACE::SavePosition();
}

bool SpecOpsMW3::LoadPosition(void *)
{
    return COMMON_FN_NAMESPACE::LoadPosition();
}

bool SpecOpsMW3::ToggleUfo(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleUfo(pParameters);
}

bool SpecOpsMW3::ToggleSecondPlayerGodMode(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleSecondPlayerGodMode(pParameters);
}

bool SpecOpsMW3::TeleportSecondPlayerToMe(void *)
{
    return COMMON_FN_NAMESPACE::TeleportSecondPlayerToMe();
}
