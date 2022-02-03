#include "pch.h"
#include "Games\SpecOps\MW2Alpha\MW2.h"

#include "Games\SpecOps\MW2Alpha\MenuFunctions.h"


bool SpecOpsAlphaMW2::s_bJumped = false;


void SpecOpsAlphaMW2::Init()
{
    Xam::XNotify("Hayzen - MW2 Alpha Spec Ops Detected");

    // Give the system some time to fully load the game in memory
    Sleep(200);

    // Set the draw function addresses
    m_dwDrawTextFnAddr = 0x82386E50;
    m_dwDrawRectangleFnAddr = 0x82386590;
    m_dwRegisterFontFnAddr = 0x82380CB0;
    m_dwRegisterMaterialFnAddr = 0x82380880;

    // Set the save and load functions to use fr the current game
    s_Menu.SetSavePositionFn(SpecOpsAlphaMW2MenuFunctions::SavePosition);
    s_Menu.SetLoadPositionFn(SpecOpsAlphaMW2MenuFunctions::LoadPosition);

    // Set the draw function pointers with the addresses above
    Game::Init();

    // Create the structure of the menu
    CreateStructure();

    // Set up the function hooks
    Memory::HookFunctionStart(reinterpret_cast<DWORD *>(0x82133BE0), reinterpret_cast<DWORD *>(SCR_DrawScreenFieldStub), reinterpret_cast<DWORD>(SCR_DrawScreenFieldHook));
    Memory::HookFunctionStart(reinterpret_cast<DWORD *>(0x821EA940), reinterpret_cast<DWORD *>(ClientCommandStub), reinterpret_cast<DWORD>(ClientCommandHook));
}

void SpecOpsAlphaMW2::CreateStructure()
{
    // Set the global title of the menu
    s_RootOption.SetText("Cod Jumper");

    // Main section
    auto pMain = MakeOption("Main", 0);
    pMain->AddChild(MakeOption("God Mode", 0, SpecOpsAlphaMW2MenuFunctions::ToggleGodMode));
    pMain->AddChild(MakeOption("Ammo", 1, SpecOpsAlphaMW2MenuFunctions::ToggleAmmo));
    pMain->AddChild(MakeOption("Jump Height", 2, SpecOpsAlphaMW2MenuFunctions::ChangeJumpHeight));
    s_RootOption.AddChild(pMain);

    // Teleport section
    auto pTeleport = MakeOption("Teleport", 1);
    pTeleport->AddChild(MakeOption("Save/Load Binds", 0, SpecOpsAlphaMW2MenuFunctions::ToggleSaveLoadBinds));
    pTeleport->AddChild(MakeOption("Save Position", 1, SpecOpsAlphaMW2MenuFunctions::SavePosition));
    pTeleport->AddChild(MakeOption("Load Position", 2, SpecOpsAlphaMW2MenuFunctions::LoadPosition));
    pTeleport->AddChild(MakeOption("UFO", 3, SpecOpsAlphaMW2MenuFunctions::ToggleUFO));
    s_RootOption.AddChild(pTeleport);

    // Second player section
    auto pSecondPlayer = MakeOption("Second Player", 2);
    pSecondPlayer->AddChild(MakeOption("God Mode", 0, SpecOpsAlphaMW2MenuFunctions::ToggleSecondPlayerGodMode));
    pSecondPlayer->AddChild(MakeOption("Teleport to Me", 1, SpecOpsAlphaMW2MenuFunctions::TeleportSecondPlayerToMe));
    s_RootOption.AddChild(pSecondPlayer);
}

void SpecOpsAlphaMW2::ClientCommandHook(int clientNum, const char *s)
{
    // Call the original ClientCommand function
    ClientCommandStub(clientNum, s);

    // Register when the user pressed the A button
    if (!strcmp(s, "notify +gostand"))
        s_bJumped = true;

    // The 'n 42' event means the game started
    if (!strcmp(s, "notify -gostand"))
    {
        // The 'n 42' event also occurs when the A button is released so, to avoid
        // resetting the menu every time the player jumps, we need to make sure the
        // 'n 7' event didn't occur just before.
        if (!s_bJumped)
        {
            // We have no way of knowing when the game ends so, if the menu was already
            // initialized, reset it first
            if (s_Menu.IsInitialized())
                s_Menu.Stop();

            // Initialize the menu
            s_Menu.Init(0, &s_RootOption);

            // Disable the unlocalized error messages when printing something in the killfeed
            SpecOpsAlphaMW2GameFunctions::Cbuf_AddText(0, "set loc_warnings 0");
            SpecOpsAlphaMW2GameFunctions::Cbuf_AddText(0, "set loc_warningsUI 0");
        }

        // Register that the user released the A button
        s_bJumped = false;
    }
}

void __declspec(naked) SpecOpsAlphaMW2::ClientCommandStub(int clientNum, const char *s)
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
