// #pragma once is intentionally missing, this file is supposed to be included multiple times
// and generating different functions every time depending on the COMMON_FN_NAMESPACE macro.

#include "Core/Context.h"

namespace COMMON_FN_NAMESPACE
{

bool ToggleGodModeMP(bool enabled)
{
    gentity_s *pPlayerEntity = GetEntity(Context::ClientNum);
    XASSERT(pPlayerEntity != nullptr);

    if (enabled)
        pPlayerEntity->flags |= 1;
    else
        pPlayerEntity->flags &= ~1;

    return true;
}

bool ToggleFallDamage(bool enabled, uintptr_t patchAddress)
{
    XASSERT(patchAddress != 0);

    Memory::Write<float>(patchAddress, enabled ? 9999.0f : 128.0f);

    return true;
}

static void SpawnCrate(const vec3 &origin, const vec3 &angles)
{
    int clientNum = Context::ClientNum;

    gentity_s *pCurrentMapCrateBrushModel = GetCurrentMapCrateBrushModel();

    // Return early if the map is not supported
    if (!pCurrentMapCrateBrushModel)
    {
        iPrintLn(clientNum, "^1You cannot spawn a Crate on this map!");
        return;
    }

    // Spawn a crate and place it at origin and facing angles
    gentity_s *pEntity = G_Spawn();
    XASSERT(pEntity != nullptr);
    pEntity->r.currentOrigin = origin;
    pEntity->r.currentAngles = angles;
#if defined(GAME_MW3)
    G_SetModel(pEntity, "com_plasticcase_trap_friendly");
#elif defined(GAME_COD4)
    G_SetModel(pEntity, "com_plasticcase_beige_big");
#elif defined(GAME_WAW)
    G_SetModel(pEntity, "war_hq_obj");
#else
    G_SetModel(pEntity, "com_plasticcase_friendly");
#endif
    SP_script_model(pEntity);

    // Make the crate solid
    // CoD4 and WaW require a separate entity for the collision
#if defined(GAME_COD4) || defined(GAME_WAW)
    gentity_s *pCollisionEntity = G_Spawn();
    XASSERT(pCollisionEntity != nullptr);
    pCollisionEntity->r.currentOrigin = origin;
    pCollisionEntity->r.currentAngles = angles;

    // An offset is needed because the base object (HQ) has default angles,
    // and when setting the current angles, they need to be relative to these default angles.
    // For example, if the default angles are (0, 90, 0) and the desired angles are (0, 150, 0),
    // currentAngles needs to be (0, 150, 0) - (0, 90, 0) = (0, 60, 0).
    pCollisionEntity->r.currentAngles.y -= pCurrentMapCrateBrushModel->r.currentAngles.y;

    // On CoD4, the crate collision is 15 units too low so we have to spawn it higher
    #ifdef GAME_COD4
    pCollisionEntity->r.currentOrigin.z += 15.0f;
    #endif

    SP_script_model(pCollisionEntity);
    pCollisionEntity->r.bmodel = 4;
    pCollisionEntity->state.index = pCurrentMapCrateBrushModel->state.index;
    int contents = pCollisionEntity->r.contents;
    SV_SetBrushModel(pCollisionEntity);
    contents |= pCollisionEntity->r.contents;
    pCollisionEntity->r.contents = contents;
    SV_LinkEntity(pEntity);
    SV_LinkEntity(pCollisionEntity);
#else
    SV_UnlinkEntity(pEntity);
    pEntity->r.bmodel = 4;
    pEntity->state.index = pCurrentMapCrateBrushModel->state.index;
    int contents = pEntity->r.contents;
    SV_SetBrushModel(pEntity);
    contents |= pEntity->r.contents;
    pEntity->r.contents = contents;
    SV_LinkEntity(pEntity);
#endif
}

void SpawnCrate()
{
    int clientNum = Context::ClientNum;

    // Get the player's current position and viewY
    playerState_s *pPlayerState = GetPlayerState(clientNum);
    XASSERT(pPlayerState != nullptr);
    const vec3 &playerOrigin = pPlayerState->origin;
    float playerViewY = pPlayerState->viewAngles.y;

    // Spawn a crate Context::CrateDistance units in front of and
    // Context::CrateHeight above the player
    vec3 crateOrigin = Math::ProjectForward(
        playerOrigin,
        Math::Radians(playerViewY),
        Context::CrateDistance
    );
    crateOrigin.z += Context::CrateHeight;
    vec3 crateAngles = vec3(0.0f, playerViewY + Context::CrateAngle, 0.0f);

    SpawnCrate(crateOrigin, crateAngles);
}

void SpawnBlocker()
{
    int clientNum = Context::ClientNum;

    // Get the player's current position and viewY
    playerState_s *pPlayerState = GetPlayerState(clientNum);
    XASSERT(pPlayerState != nullptr);
    const vec3 &playerOrigin = pPlayerState->origin;
    float playerViewY = pPlayerState->viewAngles.y;

    // Spawn a standing crate 40 units in front of the player
    vec3 crateOrigin = Math::ProjectForward(
        playerOrigin,
        Math::Radians(playerViewY),
        40.0f
    );
    crateOrigin.z += 40.0f;
    vec3 crateAngles = vec3(90.0f, playerViewY, 0.0f);

    SpawnCrate(crateOrigin, crateAngles);
}

bool ChangeCratePositionPresets(size_t index)
{
    CratePositionPresets positionPresets = static_cast<CratePositionPresets>(index);

    switch (positionPresets)
    {
    case CratePosition_BounceTest:
        Context::CrateDistance = 150.0f;
        Context::CrateHeight = 0.0f;
        break;
    case CratePosition_MegaBounce:
        Context::CrateDistance = 860.0f;
        Context::CrateHeight = 1250.0f;
        break;
    case CratePosition_Floor:
        Context::CrateDistance = 0.0f;
        Context::CrateHeight = -60.0f;
        break;
    default:
        return false;
    }

    Context::CratePositionPresets = positionPresets;

    return true;
}

typedef enum _CrateOrientation
{
    CrateOrientation_Straight,
    CrateOrientation_LeftStrafe,
    CrateOrientation_RightStrafe,
} CrateOrientation;

bool ChangeCrateOrientation(size_t index)
{
    CrateOrientation orientation = static_cast<CrateOrientation>(index);

    switch (orientation)
    {
    case CrateOrientation_Straight:
        Context::CrateAngle = 0.0f;
        break;
    case CrateOrientation_LeftStrafe:
        Context::CrateAngle = -45.0f;
        break;
    case CrateOrientation_RightStrafe:
        Context::CrateAngle = 45.0f;
        break;
    default:
        return false;
    }

    return true;
}

#ifndef GAME_ALPHAGHOSTS
static void SetBotMovement(bool enabled)
{
    XASSERT(Context::pBotEntity != nullptr);

    // This dvar is protected on MW3 so it can only be set via a console command
    #if defined(GAME_MW3)
    Cbuf_AddText(0, Formatter::Format("set testClients_doMove %s", enabled ? "1" : "0").c_str());
    #elif defined(GAME_COD4)
    static_cast<gentity_s *>(Context::pBotEntity)->client->bFrozen = !enabled;
    #elif defined(GAME_WAW)
    SetClientDvar(-1, "sv_botsRandomInput", enabled ? "1" : "0");
    #else
    SetClientDvar(-1, "testClients_doMove", enabled ? "1" : "0");
    #endif
}

static void SetBotAttack(bool enabled)
{
    // This dvar is protected on MW3 so it can only be set via a console command
    #if defined(GAME_MW3)
    Cbuf_AddText(0, Formatter::Format("set testClients_doAttack %s", enabled ? "1" : "0").c_str());
    #elif defined(GAME_WAW)
    SetClientDvar(-1, "sv_botsPressAttackBtn", enabled ? "1" : "0");
    #else
    SetClientDvar(-1, "testClients_doAttack", enabled ? "1" : "0");
    #endif
}

// Options passed to the SpawnBot function. This structure needs to be heap allocated because it will be
// used in another thread which will execute after the scope where the structure is created ends. The threaded
// function deletes the structure after using it.
struct SpawnBotOptions
{
    uintptr_t ServerIdAddress;
    uintptr_t ClientsBaseAddress;
};

void TeleportBotToMe();

uint32_t SpawnBotThread(SpawnBotOptions *pOptions)
{
    XASSERT(pOptions != nullptr);
    XASSERT(pOptions->ServerIdAddress != 0);
    XASSERT(pOptions->ClientsBaseAddress != 0);
    XASSERT(Context::pBotEntity != nullptr);

    Sleep(150);

    int serverId = Memory::Read<int>(pOptions->ServerIdAddress);

    // Prepare the commands to send to SV_ExecuteClientCommand
    #if defined(GAME_ALPHAMW2)
    std::string chooseTeamCommand = Formatter::Format("mr %i 4 autoassign", serverId);
    std::string chooseClassCommand = Formatter::Format("mr %i 11 class0", serverId);
    #elif defined(GAME_COD4) || defined(GAME_WAW)
    std::string chooseTeamCommand = Formatter::Format("mr %i 4 autoassign", serverId);
    // TODO: make the class name dynamic based on the match type (offline or online)
    std::string chooseClassCommand = Formatter::Format("mr %i 13 offline_class1_mp,0", serverId);
    #else
    std::string chooseTeamCommand = Formatter::Format("mr %i 3 autoassign", serverId);
    std::string chooseClassCommand = Formatter::Format("mr %i 10 class0", serverId);
    #endif

    // Get the address of the bot to pass to SV_ExecuteClientCommand
    gentity_s *pBot = static_cast<gentity_s *>(Context::pBotEntity);
    int botClientNum = pBot->state.number;
    client_t *botClient = &Memory::Read<client_t *>(pOptions->ClientsBaseAddress)[botClientNum];

    #if defined(GAME_COD4) || defined(GAME_WAW)
    SV_ExecuteClientCommand(botClient, chooseTeamCommand.c_str(), 1);
    #else
    SV_ExecuteClientCommand(botClient, chooseTeamCommand.c_str(), 1, 0);
    #endif
    Sleep(150);

    #if defined(GAME_COD4) || defined(GAME_WAW)
    SV_ExecuteClientCommand(botClient, chooseClassCommand.c_str(), 1);
    #else
    SV_ExecuteClientCommand(botClient, chooseClassCommand.c_str(), 1, 0);
    #endif
    Sleep(150);

    // Make the bot stand completely still
    SetBotMovement(false);
    SetBotAttack(false);

    TeleportBotToMe();

    // The options were heap allocated to live long enough to still be available in this thread
    // so we need to free them
    delete pOptions;

    return 0;
}

void SpawnBot(SpawnBotOptions *pOptions)
{
    XASSERT(pOptions != nullptr);

    gentity_s *pBot = static_cast<gentity_s *>(Context::pBotEntity);

    // Prevent the user from spawning multiple bots
    if (pBot != nullptr)
    {
        iPrintLn(Context::ClientNum, "^1There is already a bot in the game!");
        return;
    }

    pBot = SV_AddTestClient();
    Context::pBotEntity = pBot;

    // The rest of the code needs to execute on a separate thread because we need to
    // wait between certain operations. If this wasn't done on a separate thread, it
    // would block the game's thread and make it crash.
    Thread(reinterpret_cast<PTHREAD_START_ROUTINE>(SpawnBotThread), pOptions);
}

void TeleportBotToMe()
{
    int clientNum = Context::ClientNum;

    gentity_s *pBot = static_cast<gentity_s *>(Context::pBotEntity);

    // Make sure there is a bot in the game
    if (pBot == nullptr)
    {
        iPrintLn(clientNum, "^1There is no bot in the game!");
        return;
    }

    // Get the player's current position and viewY
    playerState_s *pPlayerState = GetPlayerState(clientNum);
    XASSERT(pPlayerState != nullptr);
    vec3 origin = pPlayerState->origin;
    float viewY = pPlayerState->viewAngles.y;

    // Teleport the bot in front of the player
    float distance = 100.0f;
    vec3 newOrigin = Math::ProjectForward(origin, Math::Radians(viewY), distance);
    SetClientOrigin(pBot, reinterpret_cast<const float *>(&newOrigin));
}

bool ToggleBotMovement(bool enabled)
{
    int clientNum = Context::ClientNum;

    gentity_s *pBot = static_cast<gentity_s *>(Context::pBotEntity);

    // Make sure there is a bot in the game
    if (pBot == nullptr)
    {
        iPrintLn(clientNum, "^1There is no bot in the game!");
        return false;
    }

    SetBotMovement(enabled);

    return true;
}

bool ToggleBotAttack(bool enabled)
{
    int clientNum = Context::ClientNum;

    gentity_s *pBot = static_cast<gentity_s *>(Context::pBotEntity);

    // Make sure there is a bot in the game
    if (pBot == nullptr)
    {
        iPrintLn(clientNum, "^1There is no bot in the game!");
        return false;
    }

    SetBotAttack(enabled);

    return true;
}
#endif

}
