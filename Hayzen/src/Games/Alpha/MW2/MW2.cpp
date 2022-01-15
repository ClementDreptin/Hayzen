#include "pch.h"
#include "Games\Alpha\MW2\MW2.h"

#include "Games\Alpha\MW2\MenuFunctions.h"


void AlphaMW2::Init()
{
    Xam::XNotify("Hayzen - MW2 Alpha Multiplayer Detected");

    // Give the system some time to fully load the game in memory
    Sleep(200);

    // Set the draw function addresses
    m_dwDrawTextFnAddr = 0x823BB4D8;
    m_dwDrawRectangleFnAddr = 0x823BAC18;
    m_dwRegisterFontFnAddr = 0x823B6D58;
    m_dwRegisterMaterialFnAddr = 0x823B6928;

    // Set the save and load functions to use fr the current game
    s_Menu.SetSavePositionFn(AlphaMW2MenuFunctions::SavePosition);
    s_Menu.SetLoadPositionFn(AlphaMW2MenuFunctions::LoadPosition);

    // Set the draw function pointers with the addresses above
    Game::Init();

    // Create the structure of the menu
    CreateStructure();

    // Set up the function hooks
    Memory::HookFunctionStart(reinterpret_cast<DWORD *>(0x8218B5F0), reinterpret_cast<DWORD *>(SCR_DrawScreenFieldStub), reinterpret_cast<DWORD>(SCR_DrawScreenFieldHook));
    Memory::HookFunctionStart(reinterpret_cast<DWORD *>(0x822539C0), reinterpret_cast<DWORD *>(Scr_NotifyStub), reinterpret_cast<DWORD>(Scr_NotifyHook));
    Memory::HookFunctionStart(reinterpret_cast<DWORD *>(0x822B4700), reinterpret_cast<DWORD *>(SV_ExecuteClientCommandStub), reinterpret_cast<DWORD>(SV_ExecuteClientCommandHook));
}

void AlphaMW2::CreateStructure()
{
    // Set the global title of the menu
    s_RootOption.SetText("Cod Jumper");

    // Main section
    auto pMain = MakeOption("Main", 0);
    pMain->AddChild(MakeOption("God Mode", 0, AlphaMW2MenuFunctions::ToggleGodMode));
    pMain->AddChild(MakeOption("Fall Damage", 1, AlphaMW2MenuFunctions::ToggleFallDamage));
    pMain->AddChild(MakeOption("Ammo", 2, AlphaMW2MenuFunctions::ToggleAmmo));
    pMain->AddChild(MakeOption("Spawn Care Package", 3, AlphaMW2MenuFunctions::SpawnCP));
    s_RootOption.AddChild(pMain);

    // Teleport section
    auto pTeleport = MakeOption("Teleport", 1);
    pTeleport->AddChild(MakeOption("Save/Load Binds", 0, AlphaMW2MenuFunctions::ToggleSaveLoadBinds));
    pTeleport->AddChild(MakeOption("Save Position", 1, AlphaMW2MenuFunctions::SavePosition));
    pTeleport->AddChild(MakeOption("Load Position", 2, AlphaMW2MenuFunctions::LoadPosition));
    pTeleport->AddChild(MakeOption("UFO", 3, AlphaMW2MenuFunctions::ToggleUFO));
    s_RootOption.AddChild(pTeleport);

    // Bot section
    auto pBot = MakeOption("Bot", 2);
    pBot->AddChild(MakeOption("Spawn Bot", 0, AlphaMW2MenuFunctions::SpawnBot));
    pBot->AddChild(MakeOption("Teleport Bot to Me", 1, AlphaMW2MenuFunctions::TeleportBotToMe));
    pBot->AddChild(MakeOption("Toggle Bot Movement", 2, AlphaMW2MenuFunctions::ToggleBotMovement));
    s_RootOption.AddChild(pBot);
}

void AlphaMW2::Scr_NotifyHook(gentity_s *entity, uint16_t stringValue, uint32_t paramCount)
{
    // Call the original Scr_Notify function
    Scr_NotifyStub(entity, stringValue, paramCount);

    // If the client is not host, no need to go further
    int iClientNum = entity->state.number;
    if (!AlphaMW2GameFunctions::IsHost(iClientNum))
        return;

    // Get the string representing the event
    const char *szNotify = AlphaMW2GameFunctions::SL_ConvertToString(stringValue);

    // "begin" can happen multiple times a game in round-based gamemodes and we don't want
    // to recreate the menu every round so we make sure it's not already initialized
    if (!strcmp(szNotify, "begin") && !s_Menu.IsInitialized())
    {
        s_Menu.Init(iClientNum, &s_RootOption);

        // Disable the unlocalized error messages when printing something in the killfeed
        int iClientNum = s_Menu.GetClientNum();
        AlphaMW2GameFunctions::SetClientDvar(iClientNum, "loc_warnings", "0");
        AlphaMW2GameFunctions::SetClientDvar(iClientNum, "loc_warningsUI", "0");
    }
}

void AlphaMW2::SV_ExecuteClientCommandHook(int client, const char *s, int clientOK, int fromOldServer)
{
    // Call the original Scr_Notify SV_ExecuteClientCommand
    SV_ExecuteClientCommandStub(client, s, clientOK, fromOldServer);

    // If the client is not host, no need to go further
    int iClientNum = (client - Memory::Read<int>(0x83577D98)) / 0x97F80;
    if (!AlphaMW2GameFunctions::IsHost(iClientNum))
        return;

    // Stop the menu when the game ends
    if (!strcmp(s, "matchdatadone"))
        s_Menu.Stop();
}

void __declspec(naked) AlphaMW2::Scr_NotifyStub(gentity_s *entity, uint16_t stringValue, uint32_t paramCount)
{
    __asm
    {
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        li r3, 1
    }
}

void __declspec(naked) AlphaMW2::SV_ExecuteClientCommandStub(int client, const char *s, int clientOK, int fromOldServer)
{
    __asm
    {
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        li r3, 2
    }
}
