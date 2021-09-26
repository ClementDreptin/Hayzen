#include "pch.h"
#include "Games\MW2\MW2.h"

#include "Games\MW2\MenuFunctions.h"


//--------------------------------------------------------------------------------------
// Name: Init()
// Desc: Set the draw function pointers and the function hooks.
//--------------------------------------------------------------------------------------
VOID MW2::Init()
{
    Xam::XNotify("Hayzen - MW2 Multiplayer Detected");

    // Give the system some time to fully load the game in memory
    Sleep(200);

    // NOP cheat protection
    Memory::Write<INT>(0x8216906C, 0x60000000);
    Memory::Write<INT>(0x821690E4, 0x60000000);

    // Set the draw function addresses
    m_dwDrawTextFnAddr = 0x82350278;
    m_dwDrawRectangleFnAddr = 0x821384D8;
    m_dwRegisterFontFnAddr = 0x8234DCB0;
    m_dwRegisterMaterialFnAddr = 0x8234E510;

    // Set the save and load functions to use fr the current game
    s_Menu.SetSavePositionFn(MW2MenuFunctions::SavePosition);
    s_Menu.SetLoadPositionFn(MW2MenuFunctions::LoadPosition);

    // Set the draw function pointers with the addresses above
    Game::Init();

    // Create the structure of the menu
    CreateStructure();

    // Set up the function hooks
    Memory::HookFunctionStart((LPDWORD)0x8214BEB8, (LPDWORD)SCR_DrawScreenFieldStub, (DWORD)SCR_DrawScreenFieldHook);
    Memory::HookFunctionStart((LPDWORD)0x82209710, (LPDWORD)Scr_NotifyStub, (DWORD)Scr_NotifyHook);
    Memory::HookFunctionStart((LPDWORD)0x82253140, (LPDWORD)SV_ExecuteClientCommandStub, (DWORD)SV_ExecuteClientCommandHook);
}


//--------------------------------------------------------------------------------------
// Name: CreateStructure()
// Desc: Create the structure of the menu and save it a static member.
//--------------------------------------------------------------------------------------
VOID MW2::CreateStructure()
{
    // Set the global title of the menu
    s_RootOption.SetText("Cod Jumper");

    // Main section
    auto pMain = MakeOption("Main", 0);
    pMain->AddChild(MakeOption("God Mode", 0, MW2MenuFunctions::ToggleGodMode));
    pMain->AddChild(MakeOption("Fall Damage", 1, MW2MenuFunctions::ToggleFallDamage));
    pMain->AddChild(MakeOption("Ammo", 2, MW2MenuFunctions::ToggleAmmo));
    pMain->AddChild(MakeOption("Elevators", 3, MW2MenuFunctions::ToggleElevators));
    pMain->AddChild(MakeOption("Spawn Care Package", 4, MW2MenuFunctions::SpawnCP));
    pMain->AddChild(MakeOption("Knockback", 5, MW2MenuFunctions::Knockback));
    s_RootOption.AddChild(pMain);

    // Teleport section
    auto pTeleport = MakeOption("Teleport", 1);
    pTeleport->AddChild(MakeOption("Save/Load Binds", 0, MW2MenuFunctions::ToggleSaveLoadBinds));
    pTeleport->AddChild(MakeOption("Save Position", 1, MW2MenuFunctions::SavePosition));
    pTeleport->AddChild(MakeOption("Load Position", 2, MW2MenuFunctions::LoadPosition));
    pTeleport->AddChild(MakeOption("UFO", 3, MW2MenuFunctions::ToggleUFO));
    s_RootOption.AddChild(pTeleport);

    // Bot section
    auto pBot = MakeOption("Bot", 2);
    pBot->AddChild(MakeOption("Spawn Bot", 0, MW2MenuFunctions::SpawnBot));
    pBot->AddChild(MakeOption("Teleport Bot to Me", 1, MW2MenuFunctions::TeleportBotToMe));
    pBot->AddChild(MakeOption("Toggle Bot Movement", 2, MW2MenuFunctions::ToggleBotMovement));
    s_RootOption.AddChild(pBot);
}


//--------------------------------------------------------------------------------------
// Name: Scr_NotifyHook()
// Desc: Initialize the menu when the game starts.
//--------------------------------------------------------------------------------------
VOID MW2::Scr_NotifyHook(gentity_s* entity, USHORT stringValue, UINT paramCount)
{
    // Call the original Scr_Notify function
    Scr_NotifyStub(entity, stringValue, paramCount);

    // If the client is not host, no need to go further
    INT iClientNum = entity->state.number;
    if (!MW2GameFunctions::IsHost(iClientNum))
        return;

    // Get the string representing the event
    LPCSTR szNotify = MW2GameFunctions::SL_ConvertToString(stringValue);
   
    // "begin" can happen multiple times a game in round-based gamemodes and we don't want
    // to recreate the menu every round so we make sure it's not already initialized
    if (!strcmp(szNotify, "begin") && !s_Menu.IsInitialized())
    {
        // Give the game some time to start the game trying to change dvars
        Sleep(5);

        // Prevent text in the killfeed from showing as UNLOCALIZED(<text>)
        MW2GameFunctions::SetClientDvar(iClientNum, "loc_warnings", "0");
        MW2GameFunctions::SetClientDvar(iClientNum, "loc_warningsUI", "0");

        // Initialize the menu
        s_Menu.Init(iClientNum, &s_RootOption);
    }
}


//--------------------------------------------------------------------------------------
// Name: SV_ExecuteClientCommandHook()
// Desc: Stop the menu when the game ends.
//--------------------------------------------------------------------------------------
VOID MW2::SV_ExecuteClientCommandHook(INT client, LPCSTR s, INT clientOK, INT fromOldServer)
{
    // Call the original Scr_Notify SV_ExecuteClientCommand
    SV_ExecuteClientCommandStub(client, s, clientOK, fromOldServer);

    // If the client is not host, no need to go further
    INT iClientNum = (client - Memory::Read<INT>(0x83623B98)) / 0x97F80;
    if (!MW2GameFunctions::IsHost(iClientNum))
        return;

    // Stop the menu when the game ends
    if (!strcmp(s, "disconnect"))
        s_Menu.Stop();
}

//--------------------------------------------------------------------------------------
// Name: Scr_NotifyStub()
// Desc: Stub to hold the original code of Scr_Notify.
//--------------------------------------------------------------------------------------
VOID __declspec(naked) MW2::Scr_NotifyStub(gentity_s* entity, USHORT stringValue, UINT paramCount)
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


//--------------------------------------------------------------------------------------
// Name: SV_ExecuteClientCommandStub()
// Desc: Stub to hold the original code of SV_ExecuteClientCommand.
//--------------------------------------------------------------------------------------
VOID __declspec(naked) MW2::SV_ExecuteClientCommandStub(INT client, LPCSTR s, INT clientOK, INT fromOldServer)
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
