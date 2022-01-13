#include "pch.h"
#include "Games\MW3\MW3.h"

#include "Games\MW3\MenuFunctions.h"


void MW3::Init()
{
    Xam::XNotify("Hayzen - MW3 Multiplayer Detected");

    // Give the system some time to fully load the game in memory
    Sleep(200);

    // NOP cheat protection
    Memory::Write<int>(0x821ABA24, 0x60000000);
    Memory::Write<int>(0x821ABA9C, 0x60000000);

    // Set the draw function addresses
    m_dwDrawTextFnAddr = 0x8241F9E0;
    m_dwDrawRectangleFnAddr = 0x8241F288;
    m_dwRegisterFontFnAddr = 0x82407A90;
    m_dwRegisterMaterialFnAddr = 0x82413F48;

    // Set the save and load functions to use fr the current game
    s_Menu.SetSavePositionFn(MW3MenuFunctions::SavePosition);
    s_Menu.SetLoadPositionFn(MW3MenuFunctions::LoadPosition);

    // Set the draw function pointers with the addresses above
    Game::Init();

    // Create the structure of the menu
    CreateStructure();

    // Set up the function hooks
    Memory::HookFunctionStart(reinterpret_cast<DWORD *>(0x8217CF90), reinterpret_cast<DWORD *>(SCR_DrawScreenFieldStub), reinterpret_cast<DWORD>(SCR_DrawScreenFieldHook));
    Memory::HookFunctionStart(reinterpret_cast<DWORD *>(0x8226AF98), reinterpret_cast<DWORD *>(Scr_NotifyStub), reinterpret_cast<DWORD>(Scr_NotifyHook));
    Memory::HookFunctionStart(reinterpret_cast<DWORD *>(0x822C78A0), reinterpret_cast<DWORD *>(SV_ExecuteClientCommandStub), reinterpret_cast<DWORD>(SV_ExecuteClientCommandHook));
}

void MW3::CreateStructure()
{
    // Set the global title of the menu
    s_RootOption.SetText("Cod Jumper");

    // Main section
    auto pMain = MakeOption("Main", 0);
    pMain->AddChild(MakeOption("God Mode", 0, MW3MenuFunctions::ToggleGodMode));
    pMain->AddChild(MakeOption("Fall Damage", 1, MW3MenuFunctions::ToggleFallDamage));
    pMain->AddChild(MakeOption("Ammo", 2, MW3MenuFunctions::ToggleAmmo));
    pMain->AddChild(MakeOption("Spawn Care Package", 3, MW3MenuFunctions::SpawnCP));
    s_RootOption.AddChild(pMain);

    // Teleport section
    auto pTeleport = MakeOption("Teleport", 1);
    pTeleport->AddChild(MakeOption("Save/Load Binds", 0, MW3MenuFunctions::ToggleSaveLoadBinds));
    pTeleport->AddChild(MakeOption("Save Position", 1, MW3MenuFunctions::SavePosition));
    pTeleport->AddChild(MakeOption("Load Position", 2, MW3MenuFunctions::LoadPosition));
    pTeleport->AddChild(MakeOption("UFO", 3, MW3MenuFunctions::ToggleUFO));
    s_RootOption.AddChild(pTeleport);
}

void MW3::Scr_NotifyHook(gentity_s *entity, uint16_t stringValue, uint32_t paramCount)
{
    // Call the original Scr_Notify function
    Scr_NotifyStub(entity, stringValue, paramCount);

    // If the client is not host, no need to go further
    int iClientNum = entity->state.number;
    if (!MW3GameFunctions::IsHost(iClientNum))
        return;

    // Get the string representing the event
    const char *szNotify = MW3GameFunctions::SL_ConvertToString(stringValue);

    // "begin" can happen multiple times a game in round-based gamemodes and we don't want
    // to recreate the menu every round so we make sure it's not already initialized
    if (!strcmp(szNotify, "begin") && !s_Menu.IsInitialized())
        s_Menu.Init(iClientNum, &s_RootOption);
}

void MW3::SV_ExecuteClientCommandHook(int client, const char *s, int clientOK, int fromOldServer)
{
    // Call the original Scr_Notify SV_ExecuteClientCommand
    SV_ExecuteClientCommandStub(client, s, clientOK, fromOldServer);

    // If the client is not host, no need to go further
    int iClientNum = (client - Memory::Read<int>(0x836C6310)) / 0x68B80;
    if (!MW3GameFunctions::IsHost(iClientNum))
        return;

    // Stop the menu when the game ends
    if (!strcmp(s, "matchdatadone"))
        s_Menu.Stop();
}

void __declspec(naked) MW3::Scr_NotifyStub(gentity_s *entity, uint16_t stringValue, uint32_t paramCount)
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

void __declspec(naked) MW3::SV_ExecuteClientCommandStub(int client, const char *s, int clientOK, int fromOldServer)
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
