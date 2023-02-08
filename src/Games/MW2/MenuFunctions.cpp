#include "pch.h"
#include "Games/MW2/MenuFunctions.h"

using namespace MW2::Game;

#ifdef COMMON_FN_NAMESPACE
    #undef COMMON_FN_NAMESPACE
#endif
#define COMMON_FN_NAMESPACE MW2Common
#include "Games/Common/CommonFunctions.h"

#define GAME_MW2
#include "Games/Common/MultiplayerFunctions.h"
#undef GAME_MW2

void MW2::ToggleGodMode(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::ToggleGodModeMP(pMenu);
}

void MW2::ToggleFallDamage(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::ToggleFallDamage(pMenu, 0x82019C48);
}

void MW2::ToggleAmmo(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::ToggleAmmoOptions options;
    options.pMenu = pMenu;
    options.patchAddress = 0x820E1724;
    options.defaultValue = 0x7D1D4850;
    options.patchValue = 0x7D284B78;

    COMMON_FN_NAMESPACE::ToggleAmmo(options);
}

void MW2::ToggleElevators(Menu *pMenu)
{
    int clientNum = pMenu->GetClientNum();

    uintptr_t branchAddress = 0x820D8360;
    uint16_t defaultValue = 0x419A;
    uint16_t modifiedValue = 0x4800;

    if (Memory::Read<uint16_t>(branchAddress) == defaultValue)
    {
        Memory::Write<uint16_t>(branchAddress, modifiedValue);
        iPrintLn(clientNum, "Elevators ^2On");
    }
    else
    {
        Memory::Write<uint16_t>(branchAddress, defaultValue);
        iPrintLn(clientNum, "Elevators ^1Off");
    }
}

void MW2::SpawnCarePackage(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::SpawnCarePackage(pMenu);
}

static uint32_t KnockbackThread(Menu *pMenu)
{
    // Get the value from the user via the virtual keyboard
    std::string value;
    uint32_t result = Xam::ShowKeyboard(L"Knockback", L"Recommended value: 30000\nDefault value: 1000", L"30000", value, 6, VKBD_LATIN_NUMERIC);

    // If the user canceled the keyboard, return early
    if (result != ERROR_SUCCESS)
        return 0;

    // If the user did not enter anything but still closed the keyboard by pressing START, set the value to its default value
    if (value.empty())
        value = "1000";

    // Set the g_knockback value to what the user entered
    SetClientDvar(-1, "g_knockback", value);

    iPrintLn(pMenu->GetClientNum(), "Knockback set to ^2" + value);

    return 0;
}

void MW2::Knockback(Menu *pMenu)
{
    // This needs to execute on a separate thread because we need to wait for the user
    // to finish typing. If this wasn't done on a separate thread, it would block the
    // game's thread and make it crash.
    Memory::Thread(reinterpret_cast<PTHREAD_START_ROUTINE>(KnockbackThread), pMenu);
}

void MW2::ToggleSaveLoadBinds(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::ToggleSaveLoadBinds(pMenu);
}

void MW2::SavePosition(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::SavePosition(pMenu);
}

void MW2::LoadPosition(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::LoadPosition(pMenu);
}

void MW2::ToggleUfo(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::ToggleUfo(pMenu);
}

void MW2::SpawnBot(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::SpawnBotOptions *pOptions = new COMMON_FN_NAMESPACE::SpawnBotOptions();
    pOptions->pMenu = pMenu;
    pOptions->serverIdAddress = 0x8360922C;
    pOptions->clientsBaseAddress = 0x83623B98;

    COMMON_FN_NAMESPACE::SpawnBot(pOptions);
}

void MW2::TeleportBotToMe(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::TeleportBotToMe(pMenu);
}

void MW2::ToggleBotMovement(Menu *pMenu)
{
    COMMON_FN_NAMESPACE::ToggleBotMovement(pMenu);
}
