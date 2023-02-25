#include "pch.h"
#include "Games/MW3/MenuFunctions.h"

#include "Games/MW3/GameFunctions.h"

using namespace MW3::Game;

#ifdef COMMON_FN_NAMESPACE
    #undef COMMON_FN_NAMESPACE
#endif
#define COMMON_FN_NAMESPACE MW3Common
#include "Games/Common/CommonFunctions.h"

#define GAME_MW3
#include "Games/Common/MultiplayerFunctions.h"
#undef GAME_MW3

bool MW3::ToggleGodMode(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleGodModeMP(pParameters);
}

bool MW3::ToggleFallDamage(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleFallDamage(pParameters, 0x82000C04);
}

bool MW3::ToggleAmmo(void *pParameters)
{
    bool enabled = *reinterpret_cast<bool *>(pParameters);

    COMMON_FN_NAMESPACE::ToggleAmmoOptions options = { 0 };
    options.Enabled = enabled;
    options.PatchAddress = 0x820F63E4;
    options.DefaultValue = 0x7D3D5050;
    options.PatchValue = 0x7D495378;

    return COMMON_FN_NAMESPACE::ToggleAmmo(options);
}

bool MW3::SpawnCarePackage(void *)
{
    return COMMON_FN_NAMESPACE::SpawnCarePackage();
}

bool MW3::ToggleSaveLoadBinds(void *)
{
    return COMMON_FN_NAMESPACE::ToggleSaveLoadBinds();
}

bool MW3::SavePosition(void *)
{
    return COMMON_FN_NAMESPACE::SavePosition();
}

bool MW3::LoadPosition(void *)
{
    return COMMON_FN_NAMESPACE::LoadPosition();
}

bool MW3::ToggleUfo(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleUfo(pParameters);
}
