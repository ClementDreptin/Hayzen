#include "pch.h"
#include "Games/AlphaGhosts/MenuFunctions.h"

#include "Games/AlphaGhosts/GameFunctions.h"
#include "Games/AlphaGhosts/AlphaGhostsTitle.h"

using namespace AlphaGhosts::Game;

#ifdef COMMON_FN_NAMESPACE
    #undef COMMON_FN_NAMESPACE
#endif
#define COMMON_FN_NAMESPACE AlphaGhostsCommon
#ifdef COMMON_TITLE
    #undef COMMON_TITLE
#endif
#define COMMON_TITLE AlphaGhostsTitle
#include "Games/Common/CommonFunctions.h"

#define GAME_ALPHAGHOSTS
#include "Games/Common/MultiplayerFunctions.h"
#undef GAME_ALPHAGHOSTS

bool AlphaGhosts::ToggleGodMode(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleGodModeMP(pParameters);
}

bool AlphaGhosts::GoThroughInvisibleBarriers(void *pParameters)
{
    XASSERT(pParameters != nullptr);

    bool enabled = *reinterpret_cast<bool *>(pParameters);

    COMMON_FN_NAMESPACE::GoThroughInvisibleBarriersOptions options = {};
    options.Enabled = enabled;
    options.PM_CheckLadderMoveAddress = 0x82387F70;
    options.PmoveSingleAddress = 0x8238AF00;

    return COMMON_FN_NAMESPACE::GoThroughInvisibleBarriers(options);
}

bool AlphaGhosts::RecordInput(void *pParameters)
{
    return COMMON_FN_NAMESPACE::RecordInput(pParameters);
}

bool AlphaGhosts::ToggleReplayInputBind(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleReplayInputBind(pParameters);
}

bool AlphaGhosts::SpawnCarePackage(void *)
{
    return COMMON_FN_NAMESPACE::SpawnCarePackage();
}

bool AlphaGhosts::SpawnBlocker(void *)
{
    return COMMON_FN_NAMESPACE::SpawnBlocker();
}

bool AlphaGhosts::ChangeCarePackagePositionPresets(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ChangeCarePackagePositionPresets(pParameters);
}

bool AlphaGhosts::ChangeCarePackageOrientation(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ChangeCarePackageOrientation(pParameters);
}

bool AlphaGhosts::ToggleSaveLoadBinds(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleSaveLoadBinds(pParameters);
}

bool AlphaGhosts::ToggleUfoBind(void *pParameters)
{
    // We have to reimplement the same logic as the common function but change the callback

    XASSERT(pParameters != nullptr);

    bool enabled = *reinterpret_cast<bool *>(pParameters);

    if (enabled)
    {
        Cbuf_AddText(0, "unbind dpad_up");
        Binds::Add(XINPUT_GAMEPAD_DPAD_UP, []() -> bool { Cbuf_AddText(0, "ufo"); return true; });

        iPrintLn(Context::ClientNum, "Press ^2UP^7 to ^2UFO");
    }
    else
    {
        Cbuf_AddText(0, "bind dpad_up \"+actionslot 1\"");
        Binds::Remove(XINPUT_GAMEPAD_DPAD_UP);
    }

    return true;
}
