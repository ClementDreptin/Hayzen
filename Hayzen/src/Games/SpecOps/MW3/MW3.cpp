#include "pch.h"
#include "Games\SpecOps\MW3\MW3.h"

#include "Games\SpecOps\MW3\MenuFunctions.h"


bool SpecOpsMW3::s_bJumped = false;


void SpecOpsMW3::Init()
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
    Memory::HookFunctionStart(reinterpret_cast<DWORD *>(0x82127090), reinterpret_cast<DWORD *>(SCR_DrawScreenFieldStub), reinterpret_cast<DWORD>(SCR_DrawScreenFieldHook));
    Memory::HookFunctionStart(reinterpret_cast<DWORD *>(0x821FEFB0), reinterpret_cast<DWORD *>(ClientCommandStub), reinterpret_cast<DWORD>(ClientCommandHook));
    Memory::HookFunctionStart(reinterpret_cast<DWORD *>(0x821FA680), reinterpret_cast<DWORD *>(PlayerCmd_AllowJumpStub), reinterpret_cast<DWORD>(PlayerCmd_AllowJumpHook));
}

void SpecOpsMW3::CreateStructure()
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

    auto pSecondPlayer = MakeOption("Second Player", 2);
    pSecondPlayer->AddChild(MakeOption("God Mode", 0, SpecOpsMW3MenuFunctions::ToggleSecondPlayerGodMode));
    pSecondPlayer->AddChild(MakeOption("Teleport to Me", 1, SpecOpsMW3MenuFunctions::TeleportSecondPlayerToMe));
    s_RootOption.AddChild(pSecondPlayer);
}

void SpecOpsMW3::ClientCommandHook(int clientNum, const char *s)
{
    // Call the original ClientCommand function
    ClientCommandStub(clientNum, s);

    // Register when the user pressed the A button
    if (!strcmp(s, "n 25"))
        s_bJumped = true;

    // The 'n 26' event means the game started
    if (!strcmp(s, "n 26"))
    {
        // The 'n 26' event also occurs when the A button is released so, to avoid
        // resetting the menu every time the player jumps, we need to make sure the
        // 'n 25' event didn't occur just before.
        if (!s_bJumped)
        {
            // We have no way of knowing when the game ends so, if the menu was already
            // initialized, reset it first
            if (s_Menu.IsInitialized())
                s_Menu.Stop();

            // Initialize the menu
            s_Menu.Init(0, &s_RootOption);
        }

        // Register that the user released the A button
        s_bJumped = false;
    }
}

void SpecOpsMW3::PlayerCmd_AllowJumpHook()
{
    // Making the PlayerCmd_AllowJump function not do anything so that you can jump in
    // missions where you normally can't. This is a bad practice and may have side effects.
    return;
}

void __declspec(naked) SpecOpsMW3::ClientCommandStub(int clientNum, const char *s)
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

void __declspec(naked) SpecOpsMW3::PlayerCmd_AllowJumpStub()
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
