#include "pch.h"
#include "Games\MW2\MW2Title.h"

#include "Games\MW2\MenuFunctions.h"

Detour *MW2Title::s_pScr_NotifyDetour = nullptr;
Detour *MW2Title::s_pSV_ExecuteClientCommandDetour = nullptr;

MW2Title::~MW2Title()
{
    delete s_pScr_NotifyDetour;
    delete s_pSV_ExecuteClientCommandDetour;
}

void MW2Title::Init()
{
    Xam::XNotify("Hayzen - MW2 Multiplayer Detected");

    // Give the system some time to fully load the game in memory
    Sleep(200);

    // NOP cheat protection
    Memory::Write<int>(0x8216906C, 0x60000000);
    Memory::Write<int>(0x821690E4, 0x60000000);

    // Set the draw function addresses
    m_dwDrawTextFnAddr = 0x82350278;
    m_dwDrawRectangleFnAddr = 0x821384D8;
    m_dwRegisterFontFnAddr = 0x8234DCB0;
    m_dwRegisterMaterialFnAddr = 0x8234E510;

    // Set the save and load functions to use fr the current game
    s_Menu.SetSavePositionFn(MW2::SavePosition);
    s_Menu.SetLoadPositionFn(MW2::LoadPosition);

    // Set the draw function pointers with the addresses above
    Title::Init();

    // Create the structure of the menu
    CreateStructure();

    // Set up the function hooks
    s_pSCR_DrawScreenFieldDetour = new Detour(0x8214BEB8, SCR_DrawScreenFieldHook);
    s_pScr_NotifyDetour = new Detour(0x82209710, Scr_NotifyHook);
    s_pSV_ExecuteClientCommandDetour = new Detour(0x82253140, SV_ExecuteClientCommandHook);
}

void MW2Title::CreateStructure()
{
    // Set the global title of the menu
    s_RootOption.SetText("Cod Jumper");

    // Main section
    auto pMain = MakeOption("Main", 0);
    pMain->AddChild(MakeOption("God Mode", 0, MW2::ToggleGodMode));
    pMain->AddChild(MakeOption("Fall Damage", 1, MW2::ToggleFallDamage));
    pMain->AddChild(MakeOption("Ammo", 2, MW2::ToggleAmmo));
    pMain->AddChild(MakeOption("Elevators", 3, MW2::ToggleElevators));
    pMain->AddChild(MakeOption("Spawn Care Package", 4, MW2::SpawnCarePackage));
    pMain->AddChild(MakeOption("Knockback", 5, MW2::Knockback));
    s_RootOption.AddChild(pMain);

    // Teleport section
    auto pTeleport = MakeOption("Teleport", 1);
    pTeleport->AddChild(MakeOption("Save/Load Binds", 0, MW2::ToggleSaveLoadBinds));
    pTeleport->AddChild(MakeOption("Save Position", 1, MW2::SavePosition));
    pTeleport->AddChild(MakeOption("Load Position", 2, MW2::LoadPosition));
    pTeleport->AddChild(MakeOption("UFO", 3, MW2::ToggleUfo));
    s_RootOption.AddChild(pTeleport);

    // Bot section
    auto pBot = MakeOption("Bot", 2);
    pBot->AddChild(MakeOption("Spawn Bot", 0, MW2::SpawnBot));
    pBot->AddChild(MakeOption("Teleport Bot to Me", 1, MW2::TeleportBotToMe));
    pBot->AddChild(MakeOption("Toggle Bot Movement", 2, MW2::ToggleBotMovement));
    s_RootOption.AddChild(pBot);
}

void MW2Title::Scr_NotifyHook(MW2::Game::gentity_s *entity, uint16_t stringValue, uint32_t paramCount)
{
    // Call the original Scr_Notify function
    s_pScr_NotifyDetour->GetOriginal<decltype(&Scr_NotifyHook)>()(entity, stringValue, paramCount);

    // If the client is not host, no need to go further
    int iClientNum = entity->state.number;
    if (!MW2::Game::IsHost(iClientNum))
        return;

    // Get the string representing the event
    const char *szNotify = MW2::Game::SL_ConvertToString(stringValue);

    // "begin" can happen multiple times a game in round-based gamemodes and we don't want
    // to recreate the menu every round so we make sure it's not already initialized
    if (!strcmp(szNotify, "begin") && !s_Menu.IsInitialized())
    {
        s_Menu.Init(iClientNum, &s_RootOption);

        // Disable the unlocalized error messages when printing something in the killfeed
        int iClientNum = s_Menu.GetClientNum();
        MW2::Game::SetClientDvar(iClientNum, "loc_warnings", "0");
        MW2::Game::SetClientDvar(iClientNum, "loc_warningsUI", "0");
    }
}

void MW2Title::SV_ExecuteClientCommandHook(int client, const char *s, int clientOK, int fromOldServer)
{
    // Call the original Scr_Notify SV_ExecuteClientCommand
    s_pSV_ExecuteClientCommandDetour->GetOriginal<decltype(&SV_ExecuteClientCommandHook)>()(client, s, clientOK, fromOldServer);

    // If the client is not host, no need to go further
    int iClientNum = (client - Memory::Read<int>(0x83623B98)) / 0x97F80;
    if (!MW2::Game::IsHost(iClientNum))
        return;

    // Stop the menu when the game ends
    if (!strcmp(s, "disconnect"))
        s_Menu.Stop();
}
