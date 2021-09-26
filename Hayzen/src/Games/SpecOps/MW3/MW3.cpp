#include "pch.h"
#include "Games\SpecOps\MW3\MW3.h"

#include "Games\SpecOps\MW3\MenuFunctions.h"


//--------------------------------------------------------------------------------------
// Name: Init()
// Desc: Set the draw function pointers and the function hooks.
//--------------------------------------------------------------------------------------
VOID SpecOpsMW3::Init()
{
    Xam::XNotify("Hayzen - MW3 Spec Ops Detected");

    // Give the system some time to fully load the game in memory
    Sleep(200);

    // Set the draw function addresses
    m_dwDrawTextFnAddr = 0x823F4E30;
    m_dwDrawRectangleFnAddr = 0x823F4878;
    m_dwRegisterFontFnAddr = 0x823DD130;
    m_dwRegisterMaterialFnAddr = 0x823E95E8;

    // Set the save and load functions to use fr the current game
    s_Menu.SetSavePositionFn(SpecOpsMW3MenuFunctions::SavePosition);
    s_Menu.SetLoadPositionFn(SpecOpsMW3MenuFunctions::LoadPosition);

    // Set the draw function pointers with the addresses above
    Game::Init();

    // Create the structure of the menu
    CreateStructure();

    // Set up the function hooks
    Memory::HookFunctionStart((LPDWORD)0x82127090, (LPDWORD)SCR_DrawScreenFieldStub, (DWORD)SCR_DrawScreenFieldHook);
    Memory::HookFunctionStart((LPDWORD)0x821FEFB0, (LPDWORD)ClientCommandStub, (DWORD)ClientCommandHook);
    Memory::HookFunctionStart((LPDWORD)0x821FA680, (LPDWORD)PlayerCmd_AllowJumpStub, (DWORD)PlayerCmd_AllowJumpHook);
}


//--------------------------------------------------------------------------------------
// Name: CreateStructure()
// Desc: Create the structure of the menu and save it a static member.
//--------------------------------------------------------------------------------------
VOID SpecOpsMW3::CreateStructure()
{
    // Set the global title of the menu
    s_RootOption.SetText("Cod Jumper");

    // Main section
    auto pMain = MakeOption("Main", 0);
    pMain->AddChild(MakeOption("God Mode", 0, SpecOpsMW3MenuFunctions::ToggleGodMode));
    pMain->AddChild(MakeOption("Ammo", 1, SpecOpsMW3MenuFunctions::ToggleAmmo));
    s_RootOption.AddChild(pMain);

    // Teleport section
    auto pTeleport = MakeOption("Teleport", 1);
    pTeleport->AddChild(MakeOption("Save/Load Binds", 0, SpecOpsMW3MenuFunctions::ToggleSaveLoadBinds));
    pTeleport->AddChild(MakeOption("Save Position", 1, SpecOpsMW3MenuFunctions::SavePosition));
    pTeleport->AddChild(MakeOption("Load Position", 2, SpecOpsMW3MenuFunctions::LoadPosition));
    pTeleport->AddChild(MakeOption("UFO", 3, SpecOpsMW3MenuFunctions::ToggleUFO));
    s_RootOption.AddChild(pTeleport);
}


//--------------------------------------------------------------------------------------
// Name: ClientCommandHook()
// Desc: Initialize the menu when the game starts.
//--------------------------------------------------------------------------------------
VOID SpecOpsMW3::ClientCommandHook(INT clientNum, LPCSTR s)
{
    // Call the original ClientCommand function
    ClientCommandStub(clientNum, s);

    // The 'n 26' event means the game started
    if (!strcmp(s, "n 26"))
    {
        // We have no way of knowing the game ends so, if the menu was already
        // initialized, reset it first
        if (s_Menu.IsInitialized())
            s_Menu.Stop();

        // Initialize the menu
        s_Menu.Init(0, &s_RootOption);
    }
}


//--------------------------------------------------------------------------------------
// Name: PlayerCmd_AllowJumpHook()
// Desc: Make the PlayerCmd_AllowJump function not do anything to be able to jump in
//       missions where you normally can't.
//--------------------------------------------------------------------------------------
VOID SpecOpsMW3::PlayerCmd_AllowJumpHook()
{
    // Making the PlayerCmd_AllowJump function not do anything so that you can jump in
    // missions where you normally can't. This is a bad practice and may have side effects.
    return;
}


//--------------------------------------------------------------------------------------
// Name: ClientCommandStub()
// Desc: Stub to hold the original code of ClientCommand.
//--------------------------------------------------------------------------------------
VOID __declspec(naked) SpecOpsMW3::ClientCommandStub(INT clientNum, LPCSTR s)
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
// Name: PlayerCmd_AllowJumpStub()
// Desc: Stub to hold the original code of PlayerCmd_AllowJump.
//--------------------------------------------------------------------------------------
VOID __declspec(naked) SpecOpsMW3::PlayerCmd_AllowJumpStub()
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
