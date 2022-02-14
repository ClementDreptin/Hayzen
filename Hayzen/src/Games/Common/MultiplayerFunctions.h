// #pragma once is intentionally missing, this file is supposed to be included multiple times
// and generating different functions every time depending on the COMMON_FN_NAMESPACE macro.

// The COMMON_FN_NO_BOTS can be defined to avoid compiling the functions to manipulate bots. Don't
// forget to undefine the macro after including this file. Example:
// #define COMMON_FN_NO_BOTS
// #include "Games\Common\MultiplayerFunctions.h"
// #undef COMMON_FN_NO_BOTS

#include "Core\Menu.h"


namespace COMMON_FN_NAMESPACE
{

// Toggle God Mode (specific to Multiplayer).
void ToggleGodModeMP(Menu *pMenu)
{
    int iClientNum = pMenu->GetClientNum();

    const int GOD_MODE_ON = 4097;
    const int GOD_MODE_OFF = 4096;

    gentity_s *pPlayerEntity = GetEntity(iClientNum);

    if (pPlayerEntity->flags == GOD_MODE_OFF)
    {
        pPlayerEntity->flags = GOD_MODE_ON;
        iPrintLn(iClientNum, "God Mode ^2On");
    }
    else
    {
        pPlayerEntity->flags = GOD_MODE_OFF;
        iPrintLn(iClientNum, "God Mode ^1Off");
    }
}

// Toggle fall damage.
void ToggleFallDamage(Menu *pMenu, DWORD dwPatchAddress)
{
    int iClientNum = pMenu->GetClientNum();

    if (Memory::Read<float>(dwPatchAddress) == 128.0f)
    {
        Memory::Write<float>(dwPatchAddress, 999.0f);
        iPrintLn(iClientNum, "Fall Damage ^2Off");
    }
    else
    {
        Memory::Write<float>(dwPatchAddress, 128.0f);
        iPrintLn(iClientNum, "Fall Damage ^1On");
    }
}

// Spawn a care package.
void SpawnCarePackage(Menu *pMenu)
{
    int iClientNum = pMenu->GetClientNum();

    gentity_s *pCurrentMapBrushModel = GetCurrentMapBrushModel();

    // Return early if the map is not supported
    if (!pCurrentMapBrushModel)
    {
        iPrintLn(iClientNum, "^1You cannot spawn a Care Package on this map!");
        return;
    }

    // Get the player's current position
    float fDistance = 150.0f;
    vec3 Origin = GetPlayerState(iClientNum)->origin;
    float fViewY = GetPlayerState(iClientNum)->viewAngles.y;

    // Spawn an entity 150 units in front of the player and oriented towards
    // where they are looking at
    gentity_s *pEntity = G_Spawn();
    pEntity->r.currentOrigin = Math::ToFront(Origin, fViewY, fDistance);
    pEntity->r.currentAngles.y = fViewY;

    // Apply the care package mesh to the entity
    G_SetModel(pEntity, "com_plasticcase_friendly");
    SP_script_model(pEntity);
    SV_UnlinkEntity(pEntity);
    pEntity->r.bmodel = 4;
    pEntity->state.index = pCurrentMapBrushModel->state.index;

    // Make the care package solid
    int iContents = pEntity->r.contents;
    SV_SetBrushModel(pEntity);
    iContents |= pEntity->r.contents;
    pEntity->r.contents = iContents;

    // Register the entity for the scene 
    SV_LinkEntity(pEntity);
}

#ifndef COMMON_FN_NO_BOTS
// Options passed to the SpawnBot function. This structure needs to be heap allocated because it will be
// used in another thread which will execute after the scope where the structure is created ends. The threaded
// function deletes the structure after using it.
struct SpawnBotOptions
{
    Menu *pMenu;
    DWORD dwServerIdAddress;
    DWORD dwClientsBaseAddress;
};

void TeleportBotToMe(Menu *pMenu);

// Threaded function that makes the bot pick a team, then pick a class.
DWORD SpawnBotThread(SpawnBotOptions *pOptions)
{
    Sleep(150);

    // Prepare the commands to send to SV_ExecuteClientCommand
    int serverId = Memory::Read<int>(pOptions->dwServerIdAddress);
    std::string strChooseTeamCmd = Formatter::Format("mr %i 3 autoassign", serverId);
    std::string strChooseClassCmd = Formatter::Format("mr %i 10 class0", serverId);

    // Get the address of the bot to pass to SV_ExecuteClientCommand
    DWORD dwBotAddr = Memory::Read<DWORD>(pOptions->dwClientsBaseAddress) + reinterpret_cast<gentity_s *>(pOptions->pMenu->GetBot())->state.number * 0x97F80;

    // Make the bot choose the opposite team and wait until it's done
    SV_ExecuteClientCommand(dwBotAddr, strChooseTeamCmd.c_str(), 1, 0);
    Sleep(150);

    // Make the bot pick the first custom class and wait until it's done
    SV_ExecuteClientCommand(dwBotAddr, strChooseClassCmd.c_str(), 1, 0);
    Sleep(150);

    // Set bot-related dvars to make it completely stand still
    SetClientDvar(-1, "testClients_doMove", "0");
    SetClientDvar(-1, "testClients_doAttack", "0");
    SetClientDvar(-1, "testClients_watchKillcam", "0");

    // Teleport the bot in front of the player
    TeleportBotToMe(pOptions->pMenu);

    // The options were heap allocated to live long enough to still be available in this thread
    // so we need to free them
    delete pOptions;

    return 0;
}

// Spawn a bot.
void SpawnBot(SpawnBotOptions *pOptions)
{
    gentity_s *pBot = reinterpret_cast<gentity_s *>(pOptions->pMenu->GetBot());

    // Prevent the user from spawning multiple bots
    if (pBot)
    {
        iPrintLn(pOptions->pMenu->GetClientNum(), "^1There is already a bot in the game!");
        return;
    }

    // Create the bot
    pBot = SV_AddTestClient();
    pOptions->pMenu->SetBot(pBot);

    // The rest of the code needs to execute on a separate thread because we need to
    // wait between certain operations. If this wasn't done on a separate thread, it
    // would block the game's thread and make it crash.
    Memory::Thread(reinterpret_cast<PTHREAD_START_ROUTINE>(SpawnBotThread), pOptions);
}

// Teleport the bot in front of the player.
void TeleportBotToMe(Menu *pMenu)
{
    int iClientNum = pMenu->GetClientNum();

    gentity_s *pBot = reinterpret_cast<gentity_s *>(pMenu->GetBot());

    // Make sure there is a bot in the game
    if (!pBot)
    {
        iPrintLn(iClientNum, "^1There is no bot in the game!");
        return;
    }

    // Get the player's current position
    float fDistance = 100.0f;
    vec3 Origin = GetPlayerState(iClientNum)->origin;
    float fViewY = GetPlayerState(iClientNum)->viewAngles.y;

    // Teleport the bot in front of the player
    pBot->client->ps.origin = Math::ToFront(Origin, fViewY, fDistance);
}

// Toggle the bot's movement.
void ToggleBotMovement(Menu *pMenu)
{
    int iClientNum = pMenu->GetClientNum();

    // Make sure there is a bot in the game
    if (!pMenu->GetBot())
    {
        iPrintLn(iClientNum, "^1There is no bot in the game!");
        return;
    }

    if (Dvar_GetBool("testClients_doMove"))
    {
        SetClientDvar(-1, "testClients_doMove", "0");
        iPrintLn(iClientNum, "Bot ^2Frozen");
    }
    else
    {
        SetClientDvar(-1, "testClients_doMove", "1");
        iPrintLn(iClientNum, "Bot ^1Unfrozen");
    }
}
#endif

}
