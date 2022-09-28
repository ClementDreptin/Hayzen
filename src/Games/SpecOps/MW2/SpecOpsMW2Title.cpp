#include "pch.h"
#include "Games\SpecOps\MW2\SpecOpsMW2Title.h"

#include "Games\SpecOps\MW2\MenuFunctions.h"

bool SpecOpsMW2Title::s_HasJumped = false;
Detour *SpecOpsMW2Title::s_pClientCommandDetour = nullptr;

SpecOpsMW2Title::~SpecOpsMW2Title()
{
    delete s_pClientCommandDetour;
}

void SpecOpsMW2Title::Init()
{
    Xam::XNotify("Hayzen - MW2 Spec Ops Detected");

    // Give the system some time to fully load the game in memory
    Sleep(200);

    // Set the draw function addresses
    m_DrawTextFnAddr = 0x82391F00;
    m_DrawRectangleFnAddr = 0x82391620;
    m_RegisterFontFnAddr = 0x8238B5A8;
    m_RegisterMaterialFnAddr = 0x8238BE08;

    // Set the save and load functions to use fr the current game
    s_Menu.SetSavePositionFn(SpecOpsMW2::SavePosition);
    s_Menu.SetLoadPositionFn(SpecOpsMW2::LoadPosition);

    // Set the draw function pointers with the addresses above
    Title::Init();

    // Create the structure of the menu
    CreateStructure();

    // Set up the function hooks
    s_pSCR_DrawScreenFieldDetour = new Detour(0x821354B0, SCR_DrawScreenFieldHook);
    s_pClientCommandDetour = new Detour(0x821EFFD0, ClientCommandHook);
}

void SpecOpsMW2Title::CreateStructure()
{
    // Set the global title of the menu
    s_RootOption.SetText("Cod Jumper");

    // Main section
    auto pMain = MakeOption("Main", 0);
    pMain->AddChild(MakeOption("God Mode", 0, SpecOpsMW2::ToggleGodMode));
    pMain->AddChild(MakeOption("Ammo", 1, SpecOpsMW2::ToggleAmmo));
    pMain->AddChild(MakeOption("Jump Height", 2, SpecOpsMW2::ChangeJumpHeight));
    s_RootOption.AddChild(pMain);

    // Teleport section
    auto pTeleport = MakeOption("Teleport", 1);
    pTeleport->AddChild(MakeOption("Save/Load Binds", 0, SpecOpsMW2::ToggleSaveLoadBinds));
    pTeleport->AddChild(MakeOption("Save Position", 1, SpecOpsMW2::SavePosition));
    pTeleport->AddChild(MakeOption("Load Position", 2, SpecOpsMW2::LoadPosition));
    pTeleport->AddChild(MakeOption("UFO", 3, SpecOpsMW2::ToggleUfo));
    s_RootOption.AddChild(pTeleport);

    // Second player section
    auto pSecondPlayer = MakeOption("Second Player", 2);
    pSecondPlayer->AddChild(MakeOption("God Mode", 0, SpecOpsMW2::ToggleSecondPlayerGodMode));
    pSecondPlayer->AddChild(MakeOption("Teleport to Me", 1, SpecOpsMW2::TeleportSecondPlayerToMe));
    s_RootOption.AddChild(pSecondPlayer);
}

void SpecOpsMW2Title::ClientCommandHook(int clientNum, const char *s)
{
    // Call the original ClientCommand function
    s_pClientCommandDetour->GetOriginal<decltype(&ClientCommandHook)>()(clientNum, s);

    // Register when the user pressed the A button
    if (!strcmp(s, "n 7"))
        s_HasJumped = true;

    // The 'n 42' event means the game started
    if (!strcmp(s, "n 42"))
    {
        // The 'n 42' event also occurs when the A button is released so, to avoid
        // resetting the menu every time the player jumps, we need to make sure the
        // 'n 7' event didn't occur just before.
        if (!s_HasJumped)
        {
            // We have no way of knowing when the game ends so, if the menu was already
            // initialized, reset it first
            if (s_Menu.IsInitialized())
                s_Menu.Stop();

            // Initialize the menu
            s_Menu.Init(0, &s_RootOption);

            // Disable the unlocalized error messages when printing something in the killfeed
            SpecOpsMW2::Game::Cbuf_AddText(0, "set loc_warnings 0");
            SpecOpsMW2::Game::Cbuf_AddText(0, "set loc_warningsUI 0");
        }

        // Register that the user released the A button
        s_HasJumped = false;
    }
}
