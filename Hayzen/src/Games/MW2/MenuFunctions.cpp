#include "pch.h"
#include "Games\MW2\MenuFunctions.h"

using namespace MW2GameFunctions;


void MW2MenuFunctions::ToggleGodMode(Menu *pMenu)
{
    int iClientNum = pMenu->GetClientNum();

    const int GOD_MODE_ON = 4097;
    const int GOD_MODE_OFF = 4096;

    if (GetEntity(iClientNum)->flags == GOD_MODE_OFF)
    {
        GetEntity(iClientNum)->flags = GOD_MODE_ON;
        iPrintLn(iClientNum, "God Mode ^2On");
    }
    else
    {
        GetEntity(iClientNum)->flags = GOD_MODE_OFF;
        iPrintLn(iClientNum, "God Mode ^1Off");
    }
}


void MW2MenuFunctions::ToggleFallDamage(Menu *pMenu)
{
    int iClientNum = pMenu->GetClientNum();

    DWORD dwAddress = 0x82019C48;

    if (Memory::Read<float>(dwAddress) == 128.0f)
    {
        Memory::Write<float>(dwAddress, 9999.0f);
        iPrintLn(iClientNum, "Fall Damage ^2Off");
    }
    else
    {
        Memory::Write<float>(dwAddress, 128.0f);
        iPrintLn(iClientNum, "Fall Damage ^1On");
    }
}

void MW2MenuFunctions::ToggleAmmo(Menu *pMenu)
{
    int iClientNum = pMenu->GetClientNum();

    DWORD dwAddress = 0x820E1724;
    DWORD dwDefaultValue = 0x7D1D4850;
    DWORD dwModifiedValue = 0x7D284B78;

    if (Memory::Read<DWORD>(dwAddress) == dwDefaultValue)
    {
        Memory::Write<DWORD>(dwAddress, dwModifiedValue);
        iPrintLn(iClientNum, "Unlimited Ammo ^2On");
    }
    else
    {
        Memory::Write<DWORD>(dwAddress, dwDefaultValue);
        iPrintLn(iClientNum, "Unlimited Ammo ^1Off");
    }
}

void MW2MenuFunctions::ToggleElevators(Menu *pMenu)
{
    int iClientNum = pMenu->GetClientNum();

    DWORD dwBranchAddress = 0x820D8360;
    WORD wDefaultValue = 0x419A;
    WORD wModifiedValue = 0x4800;

    if (Memory::Read<WORD>(dwBranchAddress) == wDefaultValue)
    {
        Memory::Write<WORD>(dwBranchAddress, wModifiedValue);
        iPrintLn(iClientNum, "Elevators ^2On");
    }
    else
    {
        Memory::Write<WORD>(dwBranchAddress, wDefaultValue);
        iPrintLn(iClientNum, "Elevators ^1Off");
    }
}

void MW2MenuFunctions::SpawnCP(Menu *pMenu)
{
    int iClientNum = pMenu->GetClientNum();

    gentity_s *currentMapBrushModel = GetCurrentMapBrushModel();

    // Return early if the map is not supported
    if (!currentMapBrushModel)
    {
        iPrintLn(iClientNum, "^1You cannot spawn a Care Package on this map!");
        return;
    }

    // Get the player's current position
    float distance = 150.0f;
    vec3 origin = GetPlayerState(iClientNum)->origin;
    float viewY = GetPlayerState(iClientNum)->viewAngles.y;

    // Spawn an entity 150 units in front of the player and oriented towards
    // where they are looking at
    gentity_s *entity = G_Spawn();
    entity->r.currentOrigin = Math::ToFront(origin, viewY, distance);
    entity->r.currentAngles.y = viewY;

    // Apply the care package mesh to the entity
    G_SetModel(entity, "com_plasticcase_friendly");
    SP_script_model(entity);
    SV_UnlinkEntity(entity);
    entity->r.bmodel = 4;
    entity->state.index = currentMapBrushModel->state.index;

    // Make the care package solid
    int contents = entity->r.contents;
    SV_SetBrushModel(entity);
    contents |= entity->r.contents;
    entity->r.contents = contents;

    // Register the entity for the scene 
    SV_LinkEntity(entity);
}

// Threaded function that prompts a keyboard and sets the knockback strength to what was entered.
static DWORD KnockbackThread(Menu *pMenu)
{
    // Get the value from the user via the virtual keyboard
    std::string strValue;
    DWORD dwResult = Xam::ShowKeyboard("Knockback", "Recommended value: 30000", "30000", strValue, 6, VKBD_LATIN_NUMERIC);

    // If the user canceled the keyboard, return early
    if (dwResult != ERROR_SUCCESS)
        return 0;

    // If the user did not enter anything but still closed the keyboard by pressing START, set the value to its default value
    if (strValue == "")
        strValue = "1000";

    // Set the g_knockback value to what the user entered
    SetClientDvar(-1, "g_knockback", strValue);

    iPrintLn(pMenu->GetClientNum(), "Knockback set to ^2" + strValue);

    return 0;
}

void MW2MenuFunctions::Knockback(Menu *pMenu)
{
    // This needs to execute on a separate thread because we need to wait for the user
    // to finish typing. If this wasn't done on a separate thread, it would block the
    // game's thread and make it crash.
    Memory::Thread(reinterpret_cast<PTHREAD_START_ROUTINE>(KnockbackThread), pMenu);
}

void MW2MenuFunctions::ToggleSaveLoadBinds(Menu *pMenu)
{
    int iClientNum = pMenu->GetClientNum();

    if (!pMenu->BindsEnabled())
    {
        Cbuf_AddText(0, "unbind button_lshldr;unbind button_rshldr");
        iPrintLn(iClientNum, "Press " CHAR_RB " to ^2Save^7 and " CHAR_LB " to ^2Load");
    }
    else
    {
        Cbuf_AddText(0, "bind button_lshldr \"+smoke\";bind button_rshldr \"+frag\"");
        iPrintLn(iClientNum, "Save and Load binds ^1Off");
    }

    pMenu->ToggleBinds();
}

void MW2MenuFunctions::SavePosition(Menu *pMenu)
{
    int iClientNum = pMenu->GetClientNum();

    pMenu->SetSavedPos(GetPlayerState(iClientNum)->origin);
    pMenu->SetSavedAngles(GetPlayerState(iClientNum)->viewAngles);

    iPrintLn(iClientNum, "Position ^2Saved");
}

void MW2MenuFunctions::LoadPosition(Menu *pMenu)
{
    int iClientNum = pMenu->GetClientNum();

    const vec3 &SavedPos = pMenu->GetSavedPos();
    const vec3 &SavedAngles = pMenu->GetSavedAngles();

    // Make sure the player previously saved their position
    if (SavedPos.isNull() || SavedAngles.isNull())
    {
        iPrintLn(iClientNum, "^1Save a position first!");
        return;
    }

    gentity_s *pPlayerEntity = GetEntity(iClientNum);

    SetClientOrigin(pPlayerEntity, reinterpret_cast<const float *>(&SavedPos));
    SetClientViewAngle(pPlayerEntity, reinterpret_cast<const float *>(&SavedAngles));
}

void MW2MenuFunctions::ToggleUFO(Menu *pMenu)
{
    int iClientNum = pMenu->GetClientNum();

    if (GetGClient(iClientNum)->mFlags != 2)
    {
        GetGClient(iClientNum)->mFlags = 2;
        iPrintLn(iClientNum, "Ufo ^2On");
    }
    else
    {
        GetGClient(iClientNum)->mFlags = 0;
        iPrintLn(iClientNum, "Ufo ^1Off");
    }
}

// Threaded function that makes the bot pick a team, then pick a class.
static DWORD SpawnBotThread(Menu *pMenu)
{
    Sleep(150);

    // Prepare the commands to send to SV_ExecuteClientCommand
    int serverId = Memory::Read<int>(0x8360922C);
    std::string strChooseTeamCmd = Formatter::Format("mr %i 3 autoassign", serverId);
    std::string strChooseClassCmd = Formatter::Format("mr %i 10 class0", serverId);

    // Get the address of the bot to pass to SV_ExecuteClientCommand
    DWORD dwBotAddr = Memory::Read<DWORD>(0x83623B98) + reinterpret_cast<gentity_s *>(pMenu->GetBot())->state.number * 0x97F80;

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
    MW2MenuFunctions::TeleportBotToMe(pMenu);

    return 0;
}

void MW2MenuFunctions::SpawnBot(Menu *pMenu)
{
    gentity_s *pBot = reinterpret_cast<gentity_s *>(pMenu->GetBot());

    // Prevent the user from spawning multiple bots
    if (pBot)
    {
        iPrintLn(pMenu->GetClientNum(), "^1There is already a bot in the game!");
        return;
    }

    // Create the bot
    pBot = SV_AddTestClient();
    pMenu->SetBot(pBot);
    
    // The rest of the code needs to execute on a separate thread because we need to
    // wait between certain operations. If this wasn't done on a separate thread, it
    // would block the game's thread and make it crash.
    Memory::Thread(reinterpret_cast<PTHREAD_START_ROUTINE>(SpawnBotThread), pMenu);
}

void MW2MenuFunctions::TeleportBotToMe(Menu *pMenu)
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

void MW2MenuFunctions::ToggleBotMovement(Menu *pMenu)
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
