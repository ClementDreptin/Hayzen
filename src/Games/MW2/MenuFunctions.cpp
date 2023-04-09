#include "pch.h"
#include "Games/MW2/MenuFunctions.h"

#include "Games/MW2/GameFunctions.h"

using namespace MW2::Game;

#ifdef COMMON_FN_NAMESPACE
    #undef COMMON_FN_NAMESPACE
#endif
#define COMMON_FN_NAMESPACE MW2Common
#include "Games/Common/CommonFunctions.h"

#define GAME_MW2
#include "Games/Common/MultiplayerFunctions.h"
#undef GAME_MW2

bool MW2::ToggleGodMode(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleGodModeMP(pParameters);
}

bool MW2::ToggleFallDamage(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleFallDamage(pParameters, 0x82019C48);
}

bool MW2::ToggleAmmo(void *pParameters)
{
    bool enabled = *reinterpret_cast<bool *>(pParameters);

    COMMON_FN_NAMESPACE::ToggleAmmoOptions options = { 0 };
    options.Enabled = enabled;
    options.PatchAddress = 0x820E1724;
    options.DefaultValue = 0x7D1D4850;
    options.PatchValue = 0x7D284B78;

    return COMMON_FN_NAMESPACE::ToggleAmmo(options);
}

bool MW2::ToggleElevators(void *pParameters)
{
    bool enabled = *reinterpret_cast<bool *>(pParameters);

    Memory::Write<uint16_t>(0x820D8360, enabled ? 0x4800 : 0x419A);

    return true;
}

bool MW2::SpawnCarePackage(void *)
{
    return COMMON_FN_NAMESPACE::SpawnCarePackage();
}

bool MW2::SpawnBlocker(void *)
{
    return COMMON_FN_NAMESPACE::SpawnBlocker();
}

bool MW2::Knockback(void *pParameters)
{
    uint32_t value = *reinterpret_cast<uint32_t *>(pParameters);

    SetClientDvar(-1, "g_knockback", std::to_string(static_cast<uint64_t>(value)));

    return true;
}

bool MW2::ToggleSaveLoadBinds(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleSaveLoadBinds(pParameters);
}

bool MW2::SavePosition(void *)
{
    return COMMON_FN_NAMESPACE::SavePosition();
}

bool MW2::LoadPosition(void *)
{
    return COMMON_FN_NAMESPACE::LoadPosition();
}

bool MW2::ToggleUfo(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleUfo(pParameters);
}

bool MW2::SpawnBot(void *pParameters)
{
    COMMON_FN_NAMESPACE::SpawnBotOptions *pOptions = new COMMON_FN_NAMESPACE::SpawnBotOptions();
    pOptions->ServerIdAddress = 0x8360922C;
    pOptions->ClientsBaseAddress = 0x83623B98;

    return COMMON_FN_NAMESPACE::SpawnBot(pOptions);
}

bool MW2::TeleportBotToMe(void *)
{
    return COMMON_FN_NAMESPACE::TeleportBotToMe();
}

bool MW2::ToggleBotMovement(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleBotMovement(pParameters);
}
