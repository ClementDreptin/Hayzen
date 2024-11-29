#include "pch.h"
#include "Games/SpecOps/MW3/SpecOpsMW3Title.h"

#include "Core/Context.h"
#include "Options/OptionGroup.h"
#include "Options/ClickOption.h"
#include "Options/ToggleOption.h"
#include "Options/RangeOption.h"
#include "Games/SpecOps/MW3/MenuFunctions.h"
#include "Games/SpecOps/MW3/GameFunctions.h"
#include "UI/UI.h"

SpecOpsMW3Title::SpecOpsMW3Title()
{
    // Give the system some time to fully load the game in memory
    // Devkits are a little slower and need more time
    Sleep(Xam::IsDevkit() ? 2000 : 200);

    InitRenderer();

    // Set up the function hooks
    s_DetourMap["SCR_DrawScreenField"] = new Detour(0x82127090, SCR_DrawScreenFieldHook);
    s_DetourMap["ClientCommand"] = new Detour(0x821FEFB0, ClientCommandHook);

    InstallHooks();

    Xam::XNotify("Hayzen - MW3 Spec Ops Detected");
}

void SpecOpsMW3Title::InitMenu()
{
    std::vector<OptionGroup> optionGroups;

    bool isUnlimitedAmmoEnabled = Memory::Read<uint32_t>(0x8235BB54) == 0x7D495378;

    // Main section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ToggleOption, "God Mode", SpecOpsMW3::ToggleGodMode, false));
        options.emplace_back(MakeOption(ToggleOption, "Ammo", SpecOpsMW3::ToggleAmmo, isUnlimitedAmmoEnabled));
        options.emplace_back(MakeOption(RangeOption<uint32_t>, "Jump Height", SpecOpsMW3::ChangeJumpHeight, 39, 0, 999, 1));
        optionGroups.emplace_back(OptionGroup("Main", options));
    }

    // Teleport section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ToggleOption, "Save/Load Binds", SpecOpsMW3::ToggleSaveLoadBinds, &Context::BindsEnabled));
        options.emplace_back(MakeOption(ClickOption, "Save Position", SpecOpsMW3::SavePosition));
        options.emplace_back(MakeOption(ClickOption, "Load Position", SpecOpsMW3::LoadPosition));
        options.emplace_back(MakeOption(ToggleOption, "UFO", SpecOpsMW3::ToggleUfo, false));
        optionGroups.emplace_back(OptionGroup("Teleport", options));
    }

    // Second player section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ToggleOption, "God Mode", SpecOpsMW3::ToggleSecondPlayerGodMode, false));
        options.emplace_back(MakeOption(ClickOption, "Teleport to Me", SpecOpsMW3::TeleportSecondPlayerToMe));
        optionGroups.emplace_back(OptionGroup("Second Player", options));
    }

    // Set the save and load functions
    Context::SavePositionFn = SpecOpsMW3::SavePosition;
    Context::LoadPositionFn = SpecOpsMW3::LoadPosition;

    m_Menu.Init(optionGroups);
}

static void ForceJumpEnabled()
{
    int clientNum = Context::ClientNum;

    SpecOpsMW3::Game::gclient_s *pClient = SpecOpsMW3::Game::GetGClient(clientNum);

    // The 19th bit of pm_flags is set when the player is NOT allowed to jump
    // so we just clear this bit to allow the player to jump again
    pClient->ps.pm_flags &= ~(1 << 18);
}

static bool hasJumped = false;

void SpecOpsMW3Title::ClientCommandHook(int clientNum, const char *s)
{
    // Call the original ClientCommand function
    s_DetourMap.at("ClientCommand")->GetOriginal<decltype(&ClientCommandHook)>()(clientNum, s);

    // Register when the user pressed the A button
    if (!strcmp(s, "n 25"))
        hasJumped = true;

    // The 'n 26' event means the game started
    if (!strcmp(s, "n 26"))
    {
        // The 'n 26' event also occurs when the A button is released so, to avoid
        // resetting the menu every time the player jumps, we need to make sure the
        // 'n 25' event didn't occur just before.
        // We also allow to initialize the menu if it's not already initialized, even if 'n 42'
        // represents a jump. This is useful when loading the plugin while already in a match,
        // which avoids having to end the match and start a new one for testing in development
        if (!hasJumped || !s_CurrentInstance->InMatch())
        {
            Context::Reset();
            Context::ClientNum = 0;

            s_CurrentInstance->InMatch(true);
            s_CurrentInstance->InitMenu();

            // Some maps prevent the player from jumping so we make sure the player is
            // always allowed to jump
            ForceJumpEnabled();
        }

        // Register that the user released the A button
        hasJumped = false;
    }
}

void SpecOpsMW3Title::InitRenderer()
{
    UI::R_AddCmdDrawStretchPic = reinterpret_cast<UI::R_ADDCMDDRAWSTRETCHPIC>(0x823F4878);
    UI::R_AddCmdDrawText = reinterpret_cast<UI::R_ADDCMDDRAWTEXT>(0x823F4E30);
    UI::R_TextWidth = reinterpret_cast<UI::R_TEXTWIDTH>(0x823DD318);
    UI::R_TextHeight = reinterpret_cast<UI::R_TEXTHEIGHT>(0x823DD320);
    UI::R_RegisterFont = reinterpret_cast<UI::R_REGISTERFONT>(0x823DD130);
    UI::Material_RegisterHandle = reinterpret_cast<UI::MATERIAL_REGISTERHANDLE>(0x823E95E8);

    Title::InitRenderer();
}
