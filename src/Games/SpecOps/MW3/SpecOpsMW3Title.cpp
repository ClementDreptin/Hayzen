#include "pch.h"
#include "Games\SpecOps\MW3\SpecOpsMW3Title.h"

#include "Games\SpecOps\MW3\MenuFunctions.h"

bool SpecOpsMW3Title::s_bJumped = false;
Detour *SpecOpsMW3Title::s_pClientCommandDetour = nullptr;
Detour *SpecOpsMW3Title::s_pPlayerCmd_AllowJumpDetour = nullptr;

SpecOpsMW3Title::~SpecOpsMW3Title()
{
    delete s_pClientCommandDetour;
    delete s_pPlayerCmd_AllowJumpDetour;
}

void SpecOpsMW3Title::Init()
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
    s_Menu.SetSavePositionFn(SpecOpsMW3::SavePosition);
    s_Menu.SetLoadPositionFn(SpecOpsMW3::LoadPosition);

    // Set the draw function pointers with the addresses above
    Title::Init();

    // Create the structure of the menu
    CreateStructure();

    // Set up the function hooks
    s_pSCR_DrawScreenFieldDetour = new Detour(0x82127090, SCR_DrawScreenFieldHook);
    s_pClientCommandDetour = new Detour(0x821FEFB0, ClientCommandHook);
    s_pPlayerCmd_AllowJumpDetour = new Detour(0x821FA680, PlayerCmd_AllowJumpHook);
}

void SpecOpsMW3Title::CreateStructure()
{
    // Set the global title of the menu
    s_RootOption.SetText("Cod Jumper");

    // Main section
    auto pMain = MakeOption("Main", 0);
    pMain->AddChild(MakeOption("God Mode", 0, SpecOpsMW3::ToggleGodMode));
    pMain->AddChild(MakeOption("Ammo", 1, SpecOpsMW3::ToggleAmmo));
    pMain->AddChild(MakeOption("Jump Height", 2, SpecOpsMW3::ChangeJumpHeight));
    s_RootOption.AddChild(pMain);

    // Teleport section
    auto pTeleport = MakeOption("Teleport", 1);
    pTeleport->AddChild(MakeOption("Save/Load Binds", 0, SpecOpsMW3::ToggleSaveLoadBinds));
    pTeleport->AddChild(MakeOption("Save Position", 1, SpecOpsMW3::SavePosition));
    pTeleport->AddChild(MakeOption("Load Position", 2, SpecOpsMW3::LoadPosition));
    pTeleport->AddChild(MakeOption("UFO", 3, SpecOpsMW3::ToggleUfo));
    s_RootOption.AddChild(pTeleport);

    // Second player section
    auto pSecondPlayer = MakeOption("Second Player", 2);
    pSecondPlayer->AddChild(MakeOption("God Mode", 0, SpecOpsMW3::ToggleSecondPlayerGodMode));
    pSecondPlayer->AddChild(MakeOption("Teleport to Me", 1, SpecOpsMW3::TeleportSecondPlayerToMe));
    s_RootOption.AddChild(pSecondPlayer);
}

void SpecOpsMW3Title::ClientCommandHook(int clientNum, const char *s)
{
    // Call the original ClientCommand function
    s_pClientCommandDetour->GetOriginal<decltype(&ClientCommandHook)>()(clientNum, s);

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

void SpecOpsMW3Title::PlayerCmd_AllowJumpHook()
{
    // Making the PlayerCmd_AllowJump function not do anything so that you can jump in
    // missions where you normally can't. This is a bad practice and may have side effects.
    return;
}
