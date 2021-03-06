#include "pch.h"
#include "Games\SpecOps\AlphaMW2\SpecOpsAlphaMW2Title.h"

#include "Games\SpecOps\AlphaMW2\MenuFunctions.h"

bool SpecOpsAlphaMW2Title::s_bJumped = false;
Detour *SpecOpsAlphaMW2Title::s_pClientCommandDetour = nullptr;

SpecOpsAlphaMW2Title::~SpecOpsAlphaMW2Title()
{
    delete s_pClientCommandDetour;
}

void SpecOpsAlphaMW2Title::Init()
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
    s_Menu.SetSavePositionFn(SpecOpsAlphaMW2::SavePosition);
    s_Menu.SetLoadPositionFn(SpecOpsAlphaMW2::LoadPosition);

    // Set the draw function pointers with the addresses above
    Title::Init();

    // Create the structure of the menu
    CreateStructure();

    // Set up the function hooks
    s_pSCR_DrawScreenFieldDetour = new Detour(0x82133BE0, SCR_DrawScreenFieldHook);
    s_pClientCommandDetour = new Detour(0x821EA940, ClientCommandHook);
}

void SpecOpsAlphaMW2Title::CreateStructure()
{
    // Set the global title of the menu
    s_RootOption.SetText("Cod Jumper");

    // Main section
    auto pMain = MakeOption("Main", 0);
    pMain->AddChild(MakeOption("God Mode", 0, SpecOpsAlphaMW2::ToggleGodMode));
    pMain->AddChild(MakeOption("Ammo", 1, SpecOpsAlphaMW2::ToggleAmmo));
    pMain->AddChild(MakeOption("Jump Height", 2, SpecOpsAlphaMW2::ChangeJumpHeight));
    s_RootOption.AddChild(pMain);

    // Teleport section
    auto pTeleport = MakeOption("Teleport", 1);
    pTeleport->AddChild(MakeOption("Save/Load Binds", 0, SpecOpsAlphaMW2::ToggleSaveLoadBinds));
    pTeleport->AddChild(MakeOption("Save Position", 1, SpecOpsAlphaMW2::SavePosition));
    pTeleport->AddChild(MakeOption("Load Position", 2, SpecOpsAlphaMW2::LoadPosition));
    pTeleport->AddChild(MakeOption("UFO", 3, SpecOpsAlphaMW2::ToggleUfo));
    s_RootOption.AddChild(pTeleport);

    // Second player section
    auto pSecondPlayer = MakeOption("Second Player", 2);
    pSecondPlayer->AddChild(MakeOption("God Mode", 0, SpecOpsAlphaMW2::ToggleSecondPlayerGodMode));
    pSecondPlayer->AddChild(MakeOption("Teleport to Me", 1, SpecOpsAlphaMW2::TeleportSecondPlayerToMe));
    s_RootOption.AddChild(pSecondPlayer);
}

void SpecOpsAlphaMW2Title::ClientCommandHook(int clientNum, const char *s)
{
    // Call the original ClientCommand function
    s_pClientCommandDetour->GetOriginal<decltype(&ClientCommandHook)>()(clientNum, s);

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
            SpecOpsAlphaMW2::Game::Cbuf_AddText(0, "set loc_warnings 0");
            SpecOpsAlphaMW2::Game::Cbuf_AddText(0, "set loc_warningsUI 0");
        }

        // Register that the user released the A button
        s_bJumped = false;
    }
}
