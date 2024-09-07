// #pragma once is intentionally missing, this file is supposed to be included multiple times
// and generating different functions every time depending on the COMMON_FN_NAMESPACE macro.

#include "Core/Context.h"

namespace COMMON_FN_NAMESPACE
{

bool ToggleGodModeMP(void *pParameters)
{
    bool enabled = *reinterpret_cast<bool *>(pParameters);

    gentity_s *pPlayerEntity = GetEntity(Context::ClientNum);

    if (enabled)
        pPlayerEntity->flags |= 1;
    else
        pPlayerEntity->flags &= ~1;

    return true;
}

bool ToggleFallDamage(void *pParameters, uintptr_t patchAddress)
{
    bool enabled = *reinterpret_cast<bool *>(pParameters);

    Memory::Write<float>(patchAddress, enabled ? 9999.0f : 128.0f);

    return true;
}

static bool SpawnCarePackage(const vec3 &origin, const vec3 &angles)
{
    int clientNum = Context::ClientNum;

    gentity_s *pCurrentMapBrushModel = GetCurrentMapBrushModel();

    // Return early if the map is not supported
    if (!pCurrentMapBrushModel)
    {
        iPrintLn(clientNum, "^1You cannot spawn a Care Package on this map!");
        return false;
    }

    // Spawn a care package and place it at origin and facing angles
    gentity_s *pEntity = G_Spawn();
    pEntity->r.currentOrigin = origin;
    pEntity->r.currentAngles = angles;

    // Apply the care package mesh to the entity
#ifdef GAME_MW3
    G_SetModel(pEntity, "com_plasticcase_trap_friendly");
#else
    G_SetModel(pEntity, "com_plasticcase_friendly");
#endif
    SP_script_model(pEntity);
    SV_UnlinkEntity(pEntity);
    pEntity->r.bmodel = 4;
    pEntity->state.index = pCurrentMapBrushModel->state.index;

    // Make the care package solid
    int contents = pEntity->r.contents;
    SV_SetBrushModel(pEntity);
    contents |= pEntity->r.contents;
    pEntity->r.contents = contents;

    // Register the entity for the scene
    SV_LinkEntity(pEntity);

    return true;
}

bool SpawnCarePackage()
{
    int clientNum = Context::ClientNum;

    // Get the player's current position and viewY
    playerState_s *pPlayerSate = GetPlayerState(clientNum);
    vec3 playerOrigin = pPlayerSate->origin;
    float playerViewY = pPlayerSate->viewAngles.y;

    // Spawn a care package 150 units in front of the player and oriented towards
    // where they are looking at
    float distance = 150.0f;
    vec3 carePackageOrigin = Math::ProjectForward(playerOrigin, Math::Radians(playerViewY), distance);
    vec3 carePackageAngles = vec3(0.0f, playerViewY, 0.0f);
    return SpawnCarePackage(carePackageOrigin, carePackageAngles);
}

bool SpawnBlocker()
{
    int clientNum = Context::ClientNum;

    // Get the player's current position and viewY
    playerState_s *pPlayerSate = GetPlayerState(clientNum);
    vec3 playerOrigin = pPlayerSate->origin;
    float playerViewY = pPlayerSate->viewAngles.y;

    // Spawn a standing care package 40 units in front of the player
    float distance = 40.0f;
    vec3 carePackageOrigin = Math::ProjectForward(playerOrigin, Math::Radians(playerViewY), distance);
    carePackageOrigin.z += 40.0f;
    vec3 carePackageAngles = vec3(90.0f, playerViewY, 0.0f);
    return SpawnCarePackage(carePackageOrigin, carePackageAngles);
}

#ifndef GAME_MW3
// Options passed to the SpawnBot function. This structure needs to be heap allocated because it will be
// used in another thread which will execute after the scope where the structure is created ends. The threaded
// function deletes the structure after using it.
struct SpawnBotOptions
{
    uintptr_t ServerIdAddress;
    uintptr_t ClientsBaseAddress;
};

bool TeleportBotToMe();

uint32_t SpawnBotThread(SpawnBotOptions *pOptions)
{
    Sleep(150);

    int serverId = Memory::Read<int>(pOptions->ServerIdAddress);

    // Prepare the commands to send to SV_ExecuteClientCommand
    #if defined(GAME_ALPHAMW2)
    std::string chooseTeamCommand = Formatter::Format("mr %i 4 autoassign", serverId);
    std::string chooseClassCommand = Formatter::Format("mr %i 11 class0", serverId);
    #elif defined(GAME_MW2) || defined(GAME_NX1)
    std::string chooseTeamCommand = Formatter::Format("mr %i 3 autoassign", serverId);
    std::string chooseClassCommand = Formatter::Format("mr %i 10 class0", serverId);
    #endif

    // Get the address of the bot to pass to SV_ExecuteClientCommand
    int botClientNum = static_cast<gentity_s *>(Context::pBotEntity)->state.number;
    client_t *botClient = &Memory::Read<client_t *>(pOptions->ClientsBaseAddress)[botClientNum];

    SV_ExecuteClientCommand(botClient, chooseTeamCommand.c_str(), 1, 0);
    Sleep(150);

    SV_ExecuteClientCommand(botClient, chooseClassCommand.c_str(), 1, 0);
    Sleep(150);

    // Set bot-related dvars to make it completely stand still
    SetClientDvar(-1, "testClients_doMove", "0");
    SetClientDvar(-1, "testClients_doAttack", "0");
    SetClientDvar(-1, "testClients_watchKillcam", "0");

    TeleportBotToMe();

    // The options were heap allocated to live long enough to still be available in this thread
    // so we need to free them
    delete pOptions;

    return 0;
}

bool SpawnBot(SpawnBotOptions *pOptions)
{
    gentity_s *pBot = static_cast<gentity_s *>(Context::pBotEntity);

    // Prevent the user from spawning multiple bots
    if (pBot != nullptr)
    {
        iPrintLn(Context::ClientNum, "^1There is already a bot in the game!");
        return false;
    }

    pBot = SV_AddTestClient();
    Context::pBotEntity = pBot;

    // The rest of the code needs to execute on a separate thread because we need to
    // wait between certain operations. If this wasn't done on a separate thread, it
    // would block the game's thread and make it crash.
    Memory::Thread(reinterpret_cast<PTHREAD_START_ROUTINE>(SpawnBotThread), pOptions);

    return true;
}

bool TeleportBotToMe()
{
    int clientNum = Context::ClientNum;

    gentity_s *pBot = static_cast<gentity_s *>(Context::pBotEntity);

    // Make sure there is a bot in the game
    if (pBot == nullptr)
    {
        iPrintLn(clientNum, "^1There is no bot in the game!");
        return false;
    }

    // Get the player's current position and viewY
    playerState_s *pPlayerState = GetPlayerState(clientNum);
    vec3 origin = pPlayerState->origin;
    float viewY = pPlayerState->viewAngles.y;

    // Teleport the bot in front of the player
    float distance = 100.0f;
    vec3 newOrigin = Math::ProjectForward(origin, Math::Radians(viewY), distance);
    SetClientOrigin(pBot, reinterpret_cast<const float *>(&newOrigin));

    return true;
}

bool ToggleBotMovement(void *pParameters)
{
    bool enabled = *reinterpret_cast<bool *>(pParameters);

    int clientNum = Context::ClientNum;

    // Make sure there is a bot in the game
    if (Context::pBotEntity == nullptr)
    {
        iPrintLn(clientNum, "^1There is no bot in the game!");
        return false;
    }

    SetClientDvar(-1, "testClients_doMove", enabled ? "0" : "1");

    return true;
}
#endif

}
