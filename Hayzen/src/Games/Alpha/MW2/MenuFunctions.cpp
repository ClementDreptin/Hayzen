#include "pch.h"
#include "Games\Alpha\MW2\MenuFunctions.h"

using namespace AlphaMW2GameFunctions;


//--------------------------------------------------------------------------------------
// Name: ToggleGodMode()
// Desc: Toggle God Mode.
//--------------------------------------------------------------------------------------
VOID AlphaMW2MenuFunctions::ToggleGodMode(Menu* pMenu)
{
    INT iClientNum = pMenu->GetClientNum();
    CONST INT GOD_MODE_ON = 4097;
    CONST INT GOD_MODE_OFF = 4096;

    if (GetEntity(iClientNum)->flags == GOD_MODE_OFF)
    {
        GetEntity(iClientNum)->flags = GOD_MODE_ON;
        pMenu->SetFeedbackText("God Mode ^2On");
    }
    else
    {
        GetEntity(iClientNum)->flags = GOD_MODE_OFF;
        pMenu->SetFeedbackText("God Mode ^1Off");
    }
}


//--------------------------------------------------------------------------------------
// Name: ToggleFallDamage()
// Desc: Toggle fall damage.
//--------------------------------------------------------------------------------------
VOID AlphaMW2MenuFunctions::ToggleFallDamage(Menu* pMenu)
{
    if (Dvar_GetFloat("bg_fallDamageMinHeight") == 128.0f)
    {
        Cbuf_AddText(0, "set bg_fallDamageMinHeight 9998");
        Cbuf_AddText(0, "set bg_fallDamageMaxHeight 9999");
        pMenu->SetFeedbackText("Fall Damage ^2Off");
    }
    else
    {
        Cbuf_AddText(0, "set bg_fallDamageMinHeight 128");
        Cbuf_AddText(0, "set bg_fallDamageMaxHeight 300");
        pMenu->SetFeedbackText("Fall Damage ^1On");
    }
}


//--------------------------------------------------------------------------------------
// Name: ToggleAmmo()
// Desc: Toggle unlimited ammo.
//--------------------------------------------------------------------------------------
VOID AlphaMW2MenuFunctions::ToggleAmmo(Menu* pMenu)
{
    if (!Dvar_GetBool("player_sustainAmmo"))
    {
        Cbuf_AddText(0, "set player_sustainAmmo 1");
        pMenu->SetFeedbackText("Unlimited Ammo ^2On");
    }
    else
    {
        Cbuf_AddText(0, "set player_sustainAmmo 0");
        pMenu->SetFeedbackText("Unlimited Ammo ^1Off");
    }
}


//--------------------------------------------------------------------------------------
// Name: SpawnCP()
// Desc: Spawn a care package.
//--------------------------------------------------------------------------------------
VOID AlphaMW2MenuFunctions::SpawnCP(Menu* pMenu)
{
    INT iClientNum = pMenu->GetClientNum();
    gentity_s* currentMapBrushModel = GetCurrentMapBrushModel();

    // Return early if the map is not supported
    if (!currentMapBrushModel)
    {
        pMenu->SetFeedbackText("^1You cannot spawn a Care Package on this map!");
        return;
    }

    // Get the player's current position
    FLOAT distance = 150.0f;
    vec3 origin = GetPlayerState(iClientNum)->origin;
    FLOAT viewY = GetPlayerState(iClientNum)->viewAngles.y;

    // Spawn an entity 150 units in front of the player and oriented towards
    // where they are looking at
    gentity_s* entity = G_Spawn();
    entity->r.currentOrigin = Math::ToFront(origin, viewY, distance);
    entity->r.currentAngles.y = viewY;

    // Apply the care package mesh to the entity
    G_SetModel(entity, "com_plasticcase_friendly");
    SP_script_model(entity);
    SV_UnlinkEntity(entity);
    entity->r.bmodel = 4;
    entity->state.index = currentMapBrushModel->state.index;

    // Make the care package solid
    INT contents = entity->r.contents;
    SV_SetBrushModel(entity);
    contents |= entity->r.contents;
    entity->r.contents = contents;

    // Register the entity for the scene 
    SV_LinkEntity(entity);
}


//--------------------------------------------------------------------------------------
// Name: ToggleSaveLoadBinds()
// Desc: Toggle save and load binds.
//--------------------------------------------------------------------------------------
VOID AlphaMW2MenuFunctions::ToggleSaveLoadBinds(Menu* pMenu)
{
    if (!pMenu->BindsEnabled())
        pMenu->SetFeedbackText("Press " CHAR_RB " to ^2Save^7 and " CHAR_LB " to ^2Load");
    else
        pMenu->SetFeedbackText("Save and Load binds ^1Off");

    pMenu->ToggleBinds();
}


//--------------------------------------------------------------------------------------
// Name: SavePosition()
// Desc: Save the current player's position.
//--------------------------------------------------------------------------------------
VOID AlphaMW2MenuFunctions::SavePosition(Menu* pMenu)
{
    INT iClientNum = pMenu->GetClientNum();

    pMenu->SetSavedPos(GetPlayerState(iClientNum)->origin);
    pMenu->SetSavedAngles(GetPlayerState(iClientNum)->viewAngles);

    pMenu->SetFeedbackText("Position ^2Saved");
}


//--------------------------------------------------------------------------------------
// Name: LoadPosition()
// Desc: Load the previously saved player's position.
//--------------------------------------------------------------------------------------
VOID AlphaMW2MenuFunctions::LoadPosition(Menu* pMenu)
{
    INT iClientNum = pMenu->GetClientNum();
    CONST vec3& SavedPos = pMenu->GetSavedPos();
    CONST vec3& SavedAngles = pMenu->GetSavedAngles();

    // Make sure the player previously saved their position
    if (SavedPos == vec3(0.0f, 0.0f, 0.0f) || SavedAngles == vec3(0.0f, 0.0f, 0.0f))
    {
        pMenu->SetFeedbackText("^1Save a position first!");
        return;
    }

    TeleportPlayer(GetEntity(iClientNum), (PFLOAT)&SavedPos, (PFLOAT)&SavedAngles);
}


//--------------------------------------------------------------------------------------
// Name: ToggleUFO()
// Desc: Toggle UFO.
//--------------------------------------------------------------------------------------
VOID AlphaMW2MenuFunctions::ToggleUFO(Menu* pMenu)
{
    INT iClientNum = pMenu->GetClientNum();

    Cbuf_AddText(iClientNum, "ufo");
}


//--------------------------------------------------------------------------------------
// Name: SpawnBotThread()
// Desc: Threaded function that makes the bot spawn, pick a team, then pick a class.
//--------------------------------------------------------------------------------------
static DWORD SpawnBotThread(Menu* pMenu)
{
    // Create the bot and wait until it joins the game
    gentity_s* pBot = SV_AddTestClient();
    pMenu->SetBot(pBot);
    Sleep(150);

    // Prepare the commands to send to SV_ExecuteClientCommand
    INT serverId = Memory::Read<INT>(0x8355D5C4);
    std::string strChooseTeamCmd = Formatter::Format("mr %i 4 autoassign", serverId);
    std::string strChooseClassCmd = Formatter::Format("mr %i 11 class0", serverId);

    // Get the address of the bot to pass to SV_ExecuteClientCommand
    DWORD dwBotAddr = Memory::Read<DWORD>(0x83577D98) + pBot->state.number * 0x97F80;

    // Make the bot choose the opposite team and wait until it's done
    SV_ExecuteClientCommand(dwBotAddr, strChooseTeamCmd.c_str(), 1, 0);
    Sleep(150);

    // Make the bot pick the first custom class and wait until it's done
    SV_ExecuteClientCommand(dwBotAddr, strChooseClassCmd.c_str(), 1, 0);
    Sleep(150);

    // Set bot-related dvars to make it completely stand still.
    // We need to way a little bit between each dvar set otherwise
    // the last two won't set.
    Cbuf_AddText(0, "set testClients_doMove 0");
    Sleep(50);
    Cbuf_AddText(0, "set testClients_doAttack 0");
    Sleep(50);
    Cbuf_AddText(0, "set testClients_watchKillcam 0");

    // Teleport the bot in front of the player
    AlphaMW2MenuFunctions::TeleportBotToMe(pMenu);

    return 0;
}


//--------------------------------------------------------------------------------------
// Name: SpawnBot()
// Desc: Spawn a bot.
//--------------------------------------------------------------------------------------
VOID AlphaMW2MenuFunctions::SpawnBot(Menu* pMenu)
{
    gentity_s* pBot = (gentity_s*)pMenu->GetBot();

    // Prevent the user from spawning multiple bots
    if (pBot)
    {
        pMenu->SetFeedbackText("^1There is already a bot in the game!");
        return;
    }

    // Creating the main plugin thread with 2 for the creation flags seems to prevent
    // certain game functions from being called and SV_AddTestClient is one of them.
    // So we create another thread with the regular Thread function to be able
    // to call SV_AddTestClient safely.
    Memory::Thread((LPTHREAD_START_ROUTINE)SpawnBotThread, pMenu);
}


//--------------------------------------------------------------------------------------
// Name: TeleportBotToMe()
// Desc: Teleport the bot in front of the player.
//--------------------------------------------------------------------------------------
VOID AlphaMW2MenuFunctions::TeleportBotToMe(Menu* pMenu)
{
    INT iClientNum = pMenu->GetClientNum();
    gentity_s* pBot = (gentity_s*)pMenu->GetBot();

    // Make sure there is a bot in the game
    if (!pBot)
    {
        pMenu->SetFeedbackText("^1There is no bot in the game!");
        return;
    }

    // Get the player's current position
    FLOAT fDistance = 100.0f;
    vec3 Origin = GetPlayerState(iClientNum)->origin;
    FLOAT fViewY = GetPlayerState(iClientNum)->viewAngles.y;

    // Teleport the bot in front of the player
    pBot->client->ps.origin = Math::ToFront(Origin, fViewY, fDistance);
}


//--------------------------------------------------------------------------------------
// Name: ToggleBotMovement()
// Desc: Toggle the bot's movement.
//--------------------------------------------------------------------------------------
VOID AlphaMW2MenuFunctions::ToggleBotMovement(Menu* pMenu)
{
    gentity_s* pBot = (gentity_s*)pMenu->GetBot();

    // Make sure there is a bot in the game
    if (!pBot)
    {
        pMenu->SetFeedbackText("^1There is no bot in the game!");
        return;
    }

    if (Dvar_GetBool("testClients_doMove"))
    {
        Cbuf_AddText(0, "set testClients_doMove 0");
        pMenu->SetFeedbackText("Bot ^2Frozen");
    }
    else
    {
        Cbuf_AddText(0, "set testClients_doMove 1");
        pMenu->SetFeedbackText("Bot ^1Unfrozen");
    }
}