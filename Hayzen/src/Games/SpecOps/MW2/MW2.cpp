#include "pch.h"
#include "Games\SpecOps\MW2\MW2.h"

#include "Games\SpecOps\MW2\MenuFunctions.h"


//--------------------------------------------------------------------------------------
// Name: Init()
// Desc: Set the draw function pointers and the function hooks.
//--------------------------------------------------------------------------------------
VOID SpecOpsMW2::Init()
{
    Xam::XNotify("Hayzen - MW2 Spec Ops Detected");

    // Give the system some time to fully load the game in memory
    Sleep(200);

    // Set the draw function addresses
    m_dwDrawTextFnAddr = 0x82391F00;
    m_dwDrawRectangleFnAddr = 0x82391620;
    m_dwRegisterFontFnAddr = 0x8238B5A8;
    m_dwRegisterMaterialFnAddr = 0x8238BE08;

    // Set the save and load functions to use fr the current game
    s_Menu.SetSavePositionFn(SpecOpsMW2MenuFunctions::SavePosition);
    s_Menu.SetLoadPositionFn(SpecOpsMW2MenuFunctions::LoadPosition);

    // Set the draw function pointers with the addresses above
    Game::Init();

    // Create the structure of the menu
    CreateStructure();

    // Set up the function hooks
    Memory::HookFunctionStart((LPDWORD)0x821354B0, (LPDWORD)SCR_DrawScreenFieldStub, (DWORD)SCR_DrawScreenFieldHook);
    Memory::HookFunctionStart((LPDWORD)0x821EFFD0, (LPDWORD)ClientCommandStub, (DWORD)ClientCommandHook);
}


//--------------------------------------------------------------------------------------
// Name: CreateStructure()
// Desc: Create the structure of the menu and save it a static member.
//--------------------------------------------------------------------------------------
VOID SpecOpsMW2::CreateStructure()
{
    // Set the global title of the menu
    s_RootOption.SetText("Cod Jumper");

    // Main section
    auto pMain = MakeOption("Main", 0);
    pMain->AddChild(MakeOption("God Mode", 0, SpecOpsMW2MenuFunctions::ToggleGodMode));
    pMain->AddChild(MakeOption("Ammo", 1, SpecOpsMW2MenuFunctions::ToggleAmmo));
    s_RootOption.AddChild(pMain);

    // Teleport section
    auto pTeleport = MakeOption("Teleport", 1);
    pTeleport->AddChild(MakeOption("Save/Load Binds", 0, SpecOpsMW2MenuFunctions::ToggleSaveLoadBinds));
    pTeleport->AddChild(MakeOption("Save Position", 1, SpecOpsMW2MenuFunctions::SavePosition));
    pTeleport->AddChild(MakeOption("Load Position", 2, SpecOpsMW2MenuFunctions::LoadPosition));
    pTeleport->AddChild(MakeOption("UFO", 3, SpecOpsMW2MenuFunctions::ToggleUFO));
    s_RootOption.AddChild(pTeleport);
}


//--------------------------------------------------------------------------------------
// Name: ClientCommandHook()
// Desc: Initialize the menu when the game starts.
//--------------------------------------------------------------------------------------
VOID SpecOpsMW2::ClientCommandHook(INT clientNum, LPCSTR s)
{
    // Call the original ClientCommand function
    ClientCommandStub(clientNum, s);

    // The 'n 42' event means the game started
    if (!strcmp(s, "n 42"))
    {
        // Give the game some time to start the game before trying to change dvars
        Sleep(5);

        // Prevent text in the killfeed from showing as UNLOCALIZED(<text>)
        SpecOpsMW2GameFunctions::Cbuf_AddText(0, "set loc_warnings 0");
        SpecOpsMW2GameFunctions::Cbuf_AddText(0, "set loc_warningsUI 0");

        // Initialize the menu
        s_Menu.Init(0, &s_RootOption);
    }
}


//--------------------------------------------------------------------------------------
// Name: ClientCommandStub()
// Desc: Stub to hold the original code of ClientCommand.
//--------------------------------------------------------------------------------------
VOID __declspec(naked) SpecOpsMW2::ClientCommandStub(INT clientNum, LPCSTR s)
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
