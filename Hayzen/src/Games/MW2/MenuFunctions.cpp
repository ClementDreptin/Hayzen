#include "pch.h"
#include "Games\MW2\MenuFunctions.h"

using namespace MW2GameFunctions;


//--------------------------------------------------------------------------------------
// Name: ToggleGodMode()
// Desc: Toggle God Mode.
//--------------------------------------------------------------------------------------
VOID MW2MenuFunctions::ToggleGodMode(Menu* pMenu)
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
VOID MW2MenuFunctions::ToggleFallDamage(Menu* pMenu)
{
    DWORD dwAddress = 0x82019C48;

    if (Memory::Read<FLOAT>(dwAddress) == 128.0f)
    {
        Memory::Write<FLOAT>(dwAddress, 9999.0f);
        pMenu->SetFeedbackText("Fall Damage ^2Off");
    }
    else
    {
        Memory::Write<FLOAT>(dwAddress, 128.0f);
        pMenu->SetFeedbackText("Fall Damage ^1On");
    }
}


//--------------------------------------------------------------------------------------
// Name: ToggleAmmo()
// Desc: Toggle unlimited ammo.
//--------------------------------------------------------------------------------------
VOID MW2MenuFunctions::ToggleAmmo(Menu* pMenu)
{
    DWORD dwAddress = 0x820E1724;
    DWORD dwDefaultValue = 0x7D1D4850;
    DWORD dwModifiedValue = 0x7D284B78;

    if (Memory::Read<DWORD>(dwAddress) == dwDefaultValue)
    {
        Memory::Write<DWORD>(dwAddress, dwModifiedValue);
        pMenu->SetFeedbackText("Unlimited Ammo ^2On");
    }
    else
    {
        Memory::Write<DWORD>(dwAddress, dwDefaultValue);
        pMenu->SetFeedbackText("Unlimited Ammo ^1Off");
    }
}


//--------------------------------------------------------------------------------------
// Name: ToggleElevators()
// Desc: Toggle elvators.
//--------------------------------------------------------------------------------------
VOID MW2MenuFunctions::ToggleElevators(Menu* pMenu)
{
    DWORD dwBranchAddress = 0x820D8360;
    WORD wDefaultValue = 0x419A;
    WORD wModifiedValue = 0x4800;

    if (Memory::Read<WORD>(dwBranchAddress) == wDefaultValue)
    {
        Memory::Write<WORD>(dwBranchAddress, wModifiedValue);
        pMenu->SetFeedbackText("Elevators ^2On");
    }
    else
    {
        Memory::Write<WORD>(dwBranchAddress, wDefaultValue);
        pMenu->SetFeedbackText("Elevators ^1Off");
    }
}


//--------------------------------------------------------------------------------------
// Name: SpawnCP()
// Desc: Spawn a care package.
//--------------------------------------------------------------------------------------
VOID MW2MenuFunctions::SpawnCP(Menu* pMenu)
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
// Name: Knockback()
// Desc: Open a keyboard to allow the user to change the knockback value.
//--------------------------------------------------------------------------------------
VOID MW2MenuFunctions::Knockback(Menu* pMenu)
{
    // Get the value from the user via the virtual keyboard
    std::string strValue = Xam::ShowKeyboard("Knockback", "Recommended value: 30000", "30000", 6, VKBD_LATIN_NUMERIC);

    // If the user did not enter anything, set the value to its default value
    if (strValue == "")
        strValue = "1000";

    // Set the g_knockback value to what the user entered
    SetClientDvar(-1, "g_knockback", strValue);

    pMenu->SetFeedbackText("Knockback set to ^2" + strValue);
}


//--------------------------------------------------------------------------------------
// Name: ToggleSaveLoadBinds()
// Desc: Toggle save and load binds.
//--------------------------------------------------------------------------------------
VOID MW2MenuFunctions::ToggleSaveLoadBinds(Menu* pMenu)
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
VOID MW2MenuFunctions::SavePosition(Menu* pMenu)
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
VOID MW2MenuFunctions::LoadPosition(Menu* pMenu)
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
VOID MW2MenuFunctions::ToggleUFO(Menu* pMenu)
{
    INT iClientNum = pMenu->GetClientNum();

    if (GetGClient(iClientNum)->mFlags != 2)
    {
        GetGClient(iClientNum)->mFlags = 2;
        pMenu->SetFeedbackText("Ufo ^2On");
    }
    else
    {
        GetGClient(iClientNum)->mFlags = 0;
        pMenu->SetFeedbackText("Ufo ^1Off");
    }
}


//--------------------------------------------------------------------------------------
// Name: SpawnBot()
// Desc: Spawn a bot.
//--------------------------------------------------------------------------------------
VOID MW2MenuFunctions::SpawnBot(Menu* pMenu)
{
    gentity_s* pBot = (gentity_s*)pMenu->GetBot();

    // Prevent the user from spawning multiple bots
    if (pBot)
    {
        pMenu->SetFeedbackText("^1There is already a bot in the game!");
        return;
    }

    // Create the bot and wait until it joins the game
    pBot = SV_AddTestClient();
    pMenu->SetBot(pBot);
    Sleep(150);

    // Prepare the commands to send to SV_ExecuteClientCommand
    INT serverId = Memory::Read<INT>(0x8360922C);
    std::string strChooseTeamCmd = Formatter::Format("mr %i 3 autoassign", serverId);
    std::string strChooseClassCmd = Formatter::Format("mr %i 10 class0", serverId);

    // Get the address of the bot to pass to SV_ExecuteClientCommand
    DWORD dwBotAddr = Memory::Read<DWORD>(0x83623B98) + pBot->state.number * 0x97F80;

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
    TeleportBotToMe(pMenu);
}


//--------------------------------------------------------------------------------------
// Name: TeleportBotToMe()
// Desc: Teleport the bot in front of the player.
//--------------------------------------------------------------------------------------
VOID MW2MenuFunctions::TeleportBotToMe(Menu* pMenu)
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
VOID MW2MenuFunctions::ToggleBotMovement(Menu* pMenu)
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
        SetClientDvar(-1, "testClients_doMove", "0");
        pMenu->SetFeedbackText("Bot ^2Frozen");
    }
    else
    {
        SetClientDvar(-1, "testClients_doMove", "1");
        pMenu->SetFeedbackText("Bot ^1Unfrozen");
    }
}