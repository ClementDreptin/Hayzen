#include "pch.h"
#include "Games\AlphaMW2\AlphaMW2Title.h"

#include "Games\AlphaMW2\MenuFunctions.h"

Detour *AlphaMW2Title::s_pScr_NotifyDetour = nullptr;
Detour *AlphaMW2Title::s_pSV_ExecuteClientCommandDetour = nullptr;

AlphaMW2Title::~AlphaMW2Title()
{
    delete s_pScr_NotifyDetour;
    delete s_pSV_ExecuteClientCommandDetour;
}

void AlphaMW2Title::Init()
{
    Xam::XNotify("Hayzen - MW2 Alpha Multiplayer Detected");

    // Give the system some time to fully load the game in memory
    Sleep(200);

    // Set the draw function addresses
    m_DrawTextFnAddr = 0x823BB4D8;
    m_DrawRectangleFnAddr = 0x823BAC18;
    m_RegisterFontFnAddr = 0x823B6D58;
    m_RegisterMaterialFnAddr = 0x823B6928;

    // Set the save and load functions to use fr the current game
    s_Menu.SetSavePositionFn(AlphaMW2::SavePosition);
    s_Menu.SetLoadPositionFn(AlphaMW2::LoadPosition);

    // Set the draw function pointers with the addresses above
    Title::Init();

    // Create the structure of the menu
    CreateStructure();

    // Set up the function hooks
    s_pSCR_DrawScreenFieldDetour = new Detour(0x8218B5F0, SCR_DrawScreenFieldHook);
    s_pScr_NotifyDetour = new Detour(0x822539C0, Scr_NotifyHook);
    s_pSV_ExecuteClientCommandDetour = new Detour(0x822B4700, SV_ExecuteClientCommandHook);
}

void AlphaMW2Title::CreateStructure()
{
    // Set the global title of the menu
    s_RootOption.SetText("Cod Jumper");

    // Main section
    auto pMain = MakeOption("Main", 0);
    pMain->AddChild(MakeOption("God Mode", 0, AlphaMW2::ToggleGodMode));
    pMain->AddChild(MakeOption("Fall Damage", 1, AlphaMW2::ToggleFallDamage));
    pMain->AddChild(MakeOption("Ammo", 2, AlphaMW2::ToggleAmmo));
    pMain->AddChild(MakeOption("Spawn Care Package", 3, AlphaMW2::SpawnCarePackage));
    s_RootOption.AddChild(pMain);

    // Teleport section
    auto pTeleport = MakeOption("Teleport", 1);
    pTeleport->AddChild(MakeOption("Save/Load Binds", 0, AlphaMW2::ToggleSaveLoadBinds));
    pTeleport->AddChild(MakeOption("Save Position", 1, AlphaMW2::SavePosition));
    pTeleport->AddChild(MakeOption("Load Position", 2, AlphaMW2::LoadPosition));
    pTeleport->AddChild(MakeOption("UFO", 3, AlphaMW2::ToggleUfo));
    s_RootOption.AddChild(pTeleport);

    // Bot section
    auto pBot = MakeOption("Bot", 2);
    pBot->AddChild(MakeOption("Spawn Bot", 0, AlphaMW2::SpawnBot));
    pBot->AddChild(MakeOption("Teleport Bot to Me", 1, AlphaMW2::TeleportBotToMe));
    pBot->AddChild(MakeOption("Toggle Bot Movement", 2, AlphaMW2::ToggleBotMovement));
    s_RootOption.AddChild(pBot);
}

void AlphaMW2Title::Scr_NotifyHook(AlphaMW2::Game::gentity_s *entity, uint16_t stringValue, uint32_t paramCount)
{
    // Call the original Scr_Notify function
    s_pScr_NotifyDetour->GetOriginal<decltype(&Scr_NotifyHook)>()(entity, stringValue, paramCount);

    // If the client is not host, no need to go further
    int clientNum = entity->state.number;
    if (!AlphaMW2::Game::IsHost(clientNum))
        return;

    // Get the string representing the event
    const char *eventName = AlphaMW2::Game::SL_ConvertToString(stringValue);

    // "begin" can happen multiple times a game in round-based gamemodes and we don't want
    // to recreate the menu every round so we make sure it's not already initialized
    if (!strcmp(eventName, "begin") && !s_Menu.IsInitialized())
    {
        s_Menu.Init(clientNum, &s_RootOption);

        // Disable the unlocalized error messages when printing something in the killfeed
        AlphaMW2::Game::SetClientDvar(clientNum, "loc_warnings", "0");
        AlphaMW2::Game::SetClientDvar(clientNum, "loc_warningsUI", "0");
    }
}

void AlphaMW2Title::SV_ExecuteClientCommandHook(int client, const char *s, int clientOK, int fromOldServer)
{
    // Call the original Scr_Notify SV_ExecuteClientCommand
    s_pSV_ExecuteClientCommandDetour->GetOriginal<decltype(&SV_ExecuteClientCommandHook)>()(client, s, clientOK, fromOldServer);

    // If the client is not host, no need to go further
    int clientNum = (client - Memory::Read<int>(0x83577D98)) / 0x97F80;
    if (!AlphaMW2::Game::IsHost(clientNum))
        return;

    // Stop the menu when the game ends
    if (!strcmp(s, "matchdatadone"))
        s_Menu.Stop();
}
