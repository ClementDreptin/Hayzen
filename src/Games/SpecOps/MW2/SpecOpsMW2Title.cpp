#include "pch.h"
#include "Games/SpecOps/MW2/SpecOpsMW2Title.h"

#include "Core/Context.h"
#include "Core/UI.h"
#include "Games/SpecOps/MW2/GameFunctions.h"
#include "Games/SpecOps/MW2/MenuFunctions.h"
#include "Modules/Binds.h"
#include "Options/ClickOption.h"
#include "Options/OptionGroup.h"
#include "Options/RangeOption.h"
#include "Options/ToggleOption.h"

SpecOpsMW2Title::SpecOpsMW2Title()
{
    // Give the system some time to fully load the game in memory
    // Devkits are a little slower and need more time
    Sleep(Xam::IsDevkit() ? 2000 : 200);

    InitRenderer();

    // Set up the function hooks
    s_DetourMap["SCR_DrawScreenField"] = Detour(0x821354B0, SCR_DrawScreenFieldHook);
    s_DetourMap["ClientCommand"] = Detour(0x821EFFD0, ClientCommandHook);

    InstallHooks();

    Xam::XNotify("Hayzen - MW2 Spec Ops Detected");
}

void SpecOpsMW2Title::InitMenu()
{
    std::vector<OptionGroup> optionGroups;

    bool isUnlimitedAmmoEnabled = Memory::Read<uint32_t>(0x82331F48) == 0x7D284B78;
    float jumpHeightValue = SpecOpsMW2::Game::Dvar_GetFloat("jump_height");
    bool goThroughInvisibleBarriersEnabled =
        s_DetourMap.find("PM_CheckLadderMove") != s_DetourMap.end() &&
        s_DetourMap.find("PmoveSingle") != s_DetourMap.end();

    // Main section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ToggleOption, "God Mode", SpecOpsMW2::ToggleGodMode, false));
        options.emplace_back(MakeOption(ToggleOption, "Ammo", SpecOpsMW2::ToggleAmmo, isUnlimitedAmmoEnabled));
        options.emplace_back(MakeOption(RangeOption<uint32_t>, "Jump Height", SpecOpsMW2::ChangeJumpHeight, static_cast<uint32_t>(jumpHeightValue), 0, 999, 1));
        options.emplace_back(MakeOption(ToggleOption, "Remove Invisible Barriers", SpecOpsMW2::GoThroughInvisibleBarriers, goThroughInvisibleBarriersEnabled));
        optionGroups.emplace_back(OptionGroup("Main", options));
    }

    // Teleport section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ToggleOption, "Save/Load Binds", SpecOpsMW2::ToggleSaveLoadBinds, false));
        options.emplace_back(MakeOption(ToggleOption, "UFO Bind", SpecOpsMW2::ToggleUfoBind, false));
        optionGroups.emplace_back(OptionGroup("Teleport", options));
    }

    // Second player section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ToggleOption, "God Mode", SpecOpsMW2::ToggleSecondPlayerGodMode, false));
        options.emplace_back(MakeOption(ClickOption, "Teleport to Me", SpecOpsMW2::TeleportSecondPlayerToMe));
        optionGroups.emplace_back(OptionGroup("2nd Player", options));
    }

    // Input Replay
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ToggleOption, "Record Input", SpecOpsMW2::RecordInput, false));
        options.emplace_back(MakeOption(ToggleOption, "Replay Input Bind", SpecOpsMW2::ToggleReplayInputBind, false));
        optionGroups.emplace_back(OptionGroup("Input", options));
    }

    m_Menu.Init(optionGroups);
}

static bool hasJumped = false;

void SpecOpsMW2Title::ClientCommandHook(int clientNum, const char *s)
{
    XASSERT(s_DetourMap.find("ClientCommand") != s_DetourMap.end());

    // Call the original ClientCommand function
    s_DetourMap.at("ClientCommand").GetOriginal<decltype(&ClientCommandHook)>()(clientNum, s);

    // Register when the user pressed the A button
    if (!strcmp(s, "n 7"))
        hasJumped = true;

    // The 'n 42' event means the game started
    if (!strcmp(s, "n 42"))
    {
        // The 'n 42' event also occurs when the A button is released so, to avoid
        // resetting the menu every time the player jumps, we need to make sure the
        // 'n 7' event didn't occur just before.
        // We also allow to initialize the menu if it's not already initialized, even if 'n 42'
        // represents a jump. This is useful when loading the plugin while already in a match,
        // which avoids having to end the match and start a new one for testing in development
        if (!hasJumped || !s_CurrentInstance->InMatch())
        {
            Context::Reset();
            Binds::Clear();

            Context::ClientNum = 0;

            s_CurrentInstance->InMatch(true);
            s_CurrentInstance->InitMenu();

            // Disable the unlocalized error messages when printing something in the killfeed
            SpecOpsMW2::Game::Cbuf_AddText(0, "set loc_warnings 0");
            SpecOpsMW2::Game::Cbuf_AddText(0, "set loc_warningsUI 0");
        }

        // Register that the user released the A button
        hasJumped = false;
    }
}

void SpecOpsMW2Title::InitRenderer()
{
    UI::R_AddCmdDrawStretchPic = reinterpret_cast<UI::R_ADDCMDDRAWSTRETCHPIC>(0x82391620);
    UI::R_AddCmdDrawText = reinterpret_cast<UI::R_ADDCMDDRAWTEXT>(0x82391F00);
    UI::R_TextWidth = reinterpret_cast<UI::R_TEXTWIDTH>(0x8238B618);
    UI::R_TextHeight = reinterpret_cast<UI::R_TEXTHEIGHT>(0x8238B708);
    UI::R_RegisterFont = reinterpret_cast<UI::R_REGISTERFONT>(0x8238B5A8);
    UI::Material_RegisterHandle = reinterpret_cast<UI::MATERIAL_REGISTERHANDLE>(0x8238BE08);

    Title::InitRenderer();
}
