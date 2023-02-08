#include "pch.h"
#include "Games/MW3/MW3Title.h"

#include "Games/MW3/MenuFunctions.h"

Detour *MW3Title::s_pScr_NotifyDetour = nullptr;
Detour *MW3Title::s_pSV_ExecuteClientCommandDetour = nullptr;

MW3Title::~MW3Title()
{
    delete s_pScr_NotifyDetour;
    delete s_pSV_ExecuteClientCommandDetour;
}

void MW3Title::Init()
{
    Xam::XNotify("Hayzen - MW3 Multiplayer Detected");

    // Give the system some time to fully load the game in memory
    Sleep(200);

    // NOP cheat protection
    Memory::Write<int>(0x821ABA24, 0x60000000);
    Memory::Write<int>(0x821ABA9C, 0x60000000);

    // Set the draw function addresses
    m_DrawTextFnAddr = 0x8241F9E0;
    m_DrawRectangleFnAddr = 0x8241F288;
    m_RegisterFontFnAddr = 0x82407A90;
    m_RegisterMaterialFnAddr = 0x82413F48;

    // Set the save and load functions to use fr the current game
    s_Menu.SetSavePositionFn(MW3::SavePosition);
    s_Menu.SetLoadPositionFn(MW3::LoadPosition);

    // Set the draw function pointers with the addresses above
    Title::Init();

    // Create the structure of the menu
    CreateStructure();

    // Set up the function hooks
    s_pSCR_DrawScreenFieldDetour = new Detour(0x8217CF90, SCR_DrawScreenFieldHook);
    s_pScr_NotifyDetour = new Detour(0x8226AF98, Scr_NotifyHook);
    s_pSV_ExecuteClientCommandDetour = new Detour(0x822C78A0, SV_ExecuteClientCommandHook);
}

void MW3Title::CreateStructure()
{
    // Set the global title of the menu
    s_RootOption.SetText("Cod Jumper");

    // Main section
    auto pMain = MakeOption("Main", 0);
    pMain->AddChild(MakeOption("God Mode", 0, MW3::ToggleGodMode));
    pMain->AddChild(MakeOption("Fall Damage", 1, MW3::ToggleFallDamage));
    pMain->AddChild(MakeOption("Ammo", 2, MW3::ToggleAmmo));
    pMain->AddChild(MakeOption("Spawn Care Package", 3, MW3::SpawnCarePackage));
    s_RootOption.AddChild(pMain);

    // Teleport section
    auto pTeleport = MakeOption("Teleport", 1);
    pTeleport->AddChild(MakeOption("Save/Load Binds", 0, MW3::ToggleSaveLoadBinds));
    pTeleport->AddChild(MakeOption("Save Position", 1, MW3::SavePosition));
    pTeleport->AddChild(MakeOption("Load Position", 2, MW3::LoadPosition));
    pTeleport->AddChild(MakeOption("UFO", 3, MW3::ToggleUfo));
    s_RootOption.AddChild(pTeleport);
}

void MW3Title::Scr_NotifyHook(MW3::Game::gentity_s *entity, uint16_t stringValue, uint32_t paramCount)
{
    // Call the original Scr_Notify function
    s_pScr_NotifyDetour->GetOriginal<decltype(&Scr_NotifyHook)>()(entity, stringValue, paramCount);

    // If the client is not host, no need to go further
    int clientNum = entity->state.number;
    if (!MW3::Game::IsHost(clientNum))
        return;

    // Get the string representing the event
    const char *eventName = MW3::Game::SL_ConvertToString(stringValue);

    // "begin" can happen multiple times a game in round-based gamemodes and we don't want
    // to recreate the menu every round so we make sure it's not already initialized
    if (!strcmp(eventName, "begin") && !s_Menu.IsInitialized())
    {
        s_Menu.Init(clientNum, &s_RootOption);

        // Disable the unlocalized error messages when printing something in the killfeed
        MW3::Game::SetClientDvar(clientNum, "loc_warnings", "0");
        MW3::Game::SetClientDvar(clientNum, "loc_warningsUI", "0");
    }
}

void MW3Title::SV_ExecuteClientCommandHook(int client, const char *s, int clientOK, int fromOldServer)
{
    // Call the original Scr_Notify SV_ExecuteClientCommand
    s_pSV_ExecuteClientCommandDetour->GetOriginal<decltype(&SV_ExecuteClientCommandHook)>()(client, s, clientOK, fromOldServer);

    // If the client is not host, no need to go further
    int clientNum = (client - Memory::Read<int>(0x836C6310)) / 0x68B80;
    if (!MW3::Game::IsHost(clientNum))
        return;

    // Stop the menu when the game ends
    if (!strcmp(s, "matchdatadone"))
        s_Menu.Stop();
}
