#include "pch.h"
#include "Games/WaW/MenuFunctions.h"

#include "Games/WaW/GameFunctions.h"
#include "Games/WaW/WaWTitle.h"

using namespace WaW::Game;

#ifdef COMMON_FN_NAMESPACE
    #undef COMMON_FN_NAMESPACE
#endif
#define COMMON_FN_NAMESPACE WaWCommon
#ifdef COMMON_TITLE
    #undef COMMON_TITLE
#endif
#define COMMON_TITLE WaWTitle
#include "Games/Common/CommonFunctions.h"

#define GAME_WAW
#include "Games/Common/MultiplayerFunctions.h"
#undef GAME_WAW

bool WaW::ToggleGodMode(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleGodModeMP(pParameters);
}

bool WaW::ToggleFallDamage(void *pParameters)
{
    // For WaW we can't use the common function because changing the constant value
    // doesn't work so we went back to the old dvar way

    XASSERT(pParameters != nullptr);

    bool enabled = Memory::Read<bool>(pParameters);

    if (enabled)
    {
        SetClientDvar(-1, "bg_fallDamageMinHeight", "9998");
        SetClientDvar(-1, "bg_fallDamageMaxHeight", "9999");
    }
    else
    {
        SetClientDvar(-1, "bg_fallDamageMinHeight", "128");
        SetClientDvar(-1, "bg_fallDamageMaxHeight", "300");
    }

    return true;
}

bool WaW::ToggleAmmo(void *pParameters)
{
    XASSERT(pParameters != nullptr);

    bool enabled = Memory::Read<bool>(pParameters);

    COMMON_FN_NAMESPACE::ToggleAmmoOptions options = {};
    options.Enabled = enabled;
    options.PatchAddress = 0x82146150;
    options.DefaultValue = 0x7D695850;
    options.PatchValue = 0x60000000;

    return COMMON_FN_NAMESPACE::ToggleAmmo(options);
}

bool WaW::ChangeJumpHeight(void *pParameters)
{
    // For WaW changing the constant dvar value doesn't work
    // so we went back to the old dvar way

    XASSERT(pParameters != nullptr);

    uint32_t value = Memory::Read<uint32_t>(pParameters);

    SetClientDvar(-1, "jump_height", std::to_string(static_cast<uint64_t>(value)));

    return true;
}

bool WaW::GoThroughInvisibleBarriers(void *pParameters)
{
    XASSERT(pParameters != nullptr);

    bool enabled = Memory::Read<bool>(pParameters);

    COMMON_FN_NAMESPACE::GoThroughInvisibleBarriersOptions options = {};
    options.Enabled = enabled;
    options.PM_CheckLadderMoveAddress = 0x8213E938;
    options.PmoveSingleAddress = 0x8213ECE8;

    return COMMON_FN_NAMESPACE::GoThroughInvisibleBarriers(options);
}

bool WaW::ToggleUncappedBounces(void *pParameters)
{
    XASSERT(pParameters != nullptr);

    bool enabled = Memory::Read<bool>(pParameters);

    SetClientDvar(-1, "player_bayonetLaunchProof", enabled ? "0" : "1");

    return true;
}

#define GAME_WAW

static bool SpawnCrate(const vec3 &origin, const vec3 &angles)
{
    // TODO: remove once the mesh offset issue is resolved

    int clientNum = Context::ClientNum;

    gentity_s *pCurrentMapCrateBrushModel = GetCurrentMapCrateBrushModel();

    // Return early if the map is not supported
    if (!pCurrentMapCrateBrushModel)
    {
        iPrintLn(clientNum, "^1You cannot spawn a Crate on this map!");
        return false;
    }

    // Spawn a crate and place it at origin and facing angles
    gentity_s *pEntity = G_Spawn();
    XASSERT(pEntity != nullptr);
    pEntity->r.currentOrigin = origin;
    pEntity->r.currentAngles = angles;
    pEntity->r.currentOrigin.z += 22.0f;
    pEntity->r.currentAngles.z += pCurrentMapCrateBrushModel->r.currentAngles.z;
    G_SetModel(pEntity, "war_hq_obj");
    SP_script_model(pEntity);

    // Make the crate solid
    // CoD4 and WaW require a separate entity for the collision
    gentity_s *pCollisionEntity = G_Spawn();
    XASSERT(pCollisionEntity != nullptr);
    pCollisionEntity->r.currentOrigin = origin;
    pCollisionEntity->r.currentAngles = angles;

    // An offset is needed because the base object (HQ) has default angles,
    // and when setting the current angles, they need to be relative to these default angles.
    // For example, if the default angles are (0, 90, 0) and the desired angles are (0, 150, 0),
    // currentAngles needs to be (0, 150, 0) - (0, 90, 0) = (0, 60, 0).
    pCollisionEntity->r.currentAngles.y -= pCurrentMapCrateBrushModel->r.currentAngles.y;

    SP_script_model(pCollisionEntity);
    pCollisionEntity->r.bmodel = 4;
    pCollisionEntity->state.index = pCurrentMapCrateBrushModel->state.index;
    int contents = pCollisionEntity->r.contents;
    SV_SetBrushModel(pCollisionEntity);
    contents |= pCollisionEntity->r.contents;
    pCollisionEntity->r.contents = contents;
    SV_LinkEntity(pEntity);
    SV_LinkEntity(pCollisionEntity);

    return true;
}

#undef GAME_WAW

bool WaW::SpawnCrate(void *)
{
    // TODO: reuse the common implementation once the mesh offset issue is resolved

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

    return ::SpawnCrate(crateOrigin, crateAngles);
}

bool WaW::SpawnBlocker(void *)
{
    // TODO: fix orientation

    return COMMON_FN_NAMESPACE::SpawnBlocker();
}

bool WaW::ChangeCratePositionPresets(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ChangeCratePositionPresets(pParameters);
}

bool WaW::ChangeCrateOrientation(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ChangeCrateOrientation(pParameters);
}

bool WaW::ToggleSaveLoadBinds(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleSaveLoadBinds(pParameters);
}

bool WaW::ToggleUfoBind(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleUfoBind(pParameters);
}

bool WaW::SpawnBot(void *)
{
    COMMON_FN_NAMESPACE::SpawnBotOptions *pOptions = new COMMON_FN_NAMESPACE::SpawnBotOptions();
    pOptions->ServerIdAddress = 0x82F9C994;
    pOptions->ClientsBaseAddress = 0x83008C80 + 0xB8010; // svs + offsetof(serverStatic_t, clients) = 0x830C0C90

    return COMMON_FN_NAMESPACE::SpawnBot(pOptions);
}

bool WaW::TeleportBotToMe(void *)
{
    return COMMON_FN_NAMESPACE::TeleportBotToMe();
}

bool WaW::ToggleBotMovement(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleBotMovement(pParameters);
}

bool WaW::ToggleBotAttack(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleBotAttack(pParameters);
}

bool WaW::RecordInput(void *pParameters)
{
    return COMMON_FN_NAMESPACE::RecordInput(pParameters);
}

bool WaW::ToggleReplayInputBind(void *pParameters)
{
    return COMMON_FN_NAMESPACE::ToggleReplayInputBind(pParameters);
}
