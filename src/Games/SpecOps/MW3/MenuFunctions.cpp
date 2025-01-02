#include "pch.h"
#include "Games/SpecOps/MW3/MenuFunctions.h"

#include "Games/SpecOps/MW3/GameFunctions.h"
#include "Games/SpecOps/MW3/SpecOpsMW3Title.h"

using namespace SpecOpsMW3::Game;

#ifdef COMMON_FN_NAMESPACE
    #undef COMMON_FN_NAMESPACE
#endif
#define COMMON_FN_NAMESPACE SpecOpsMW3Common
#ifdef COMMON_TITLE
    #undef COMMON_TITLE
#endif
#define COMMON_TITLE SpecOpsMW3Title
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
    XASSERT(pParameters != nullptr);

    bool enabled = *reinterpret_cast<bool *>(pParameters);

    COMMON_FN_NAMESPACE::ToggleAmmoOptions options = {};
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

bool SpecOpsMW3::GoThroughInvisibleBarriers(void *pParameters)
{
    XASSERT(pParameters != nullptr);

    bool enabled = *reinterpret_cast<bool *>(pParameters);

    COMMON_FN_NAMESPACE::GoThroughInvisibleBarriersOptions options = {};
    options.Enabled = enabled;
    options.PM_CheckLadderMoveAddress = 0x82351650;
    options.PmoveSingleAddress = 0x82351E58;

    return COMMON_FN_NAMESPACE::GoThroughInvisibleBarriers(options);
}

bool SpecOpsMW3::ToggleSaveLoadBinds(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleSaveLoadBinds(pParameters);
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
