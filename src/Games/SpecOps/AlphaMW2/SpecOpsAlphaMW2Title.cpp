#include "pch.h"
#include "Games/SpecOps/AlphaMW2/SpecOpsAlphaMW2Title.h"

#include "Core/Context.h"
#include "Core/UI.h"
#include "Options/OptionGroup.h"
#include "Options/ClickOption.h"
#include "Options/ToggleOption.h"
#include "Options/RangeOption.h"
#include "Games/SpecOps/AlphaMW2/MenuFunctions.h"
#include "Games/SpecOps/AlphaMW2/GameFunctions.h"

SpecOpsAlphaMW2Title::SpecOpsAlphaMW2Title()
{
    // Give the system some time to fully load the game in memory
    // Devkits are a little slower and need more time
    Sleep(Xam::IsDevkit() ? 2000 : 200);

    InitRenderer();

    // Set up the function hooks
    s_DetourMap["SCR_DrawScreenField"] = new Detour(0x82133BE0, SCR_DrawScreenFieldHook);
    s_DetourMap["ClientCommand"] = new Detour(0x821EA940, ClientCommandHook);

    InstallHooks();

    Xam::XNotify("Hayzen - MW2 Alpha Spec Ops Detected");
}

void SpecOpsAlphaMW2Title::InitMenu()
{
    std::vector<OptionGroup> optionGroups;

    bool isUnlimitedAmmoEnabled = Memory::Read<uint32_t>(0x82328610) == 0x7D284B78;

    // Main section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ToggleOption, "God Mode", SpecOpsAlphaMW2::ToggleGodMode, false));
        options.emplace_back(MakeOption(ToggleOption, "Ammo", SpecOpsAlphaMW2::ToggleAmmo, isUnlimitedAmmoEnabled));
        options.emplace_back(MakeOption(RangeOption<uint32_t>, "Jump Height", SpecOpsAlphaMW2::ChangeJumpHeight, 39, 0, 999, 1));
        optionGroups.emplace_back(OptionGroup("Main", options));
    }

    // Teleport section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ToggleOption, "Save/Load Binds", SpecOpsAlphaMW2::ToggleSaveLoadBinds, &Context::BindsEnabled));
        options.emplace_back(MakeOption(ClickOption, "Save Position", SpecOpsAlphaMW2::SavePosition));
        options.emplace_back(MakeOption(ClickOption, "Load Position", SpecOpsAlphaMW2::LoadPosition));
        options.emplace_back(MakeOption(ToggleOption, "UFO", SpecOpsAlphaMW2::ToggleUfo, false));
        optionGroups.emplace_back(OptionGroup("Teleport", options));
    }

    // Second player section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ToggleOption, "God Mode", SpecOpsAlphaMW2::ToggleSecondPlayerGodMode, false));
        options.emplace_back(MakeOption(ClickOption, "Teleport to Me", SpecOpsAlphaMW2::TeleportSecondPlayerToMe));
        optionGroups.emplace_back(OptionGroup("Second Player", options));
    }

    // Set the save and load functions
    Context::SavePositionFn = SpecOpsAlphaMW2::SavePosition;
    Context::LoadPositionFn = SpecOpsAlphaMW2::LoadPosition;

    m_Menu.Init(optionGroups);
}

static bool hasJumped = false;

void SpecOpsAlphaMW2Title::ClientCommandHook(int clientNum, const char *s)
{
    XASSERT(s_DetourMap.find("ClientCommand") != s_DetourMap.end());

    // Call the original ClientCommand function
    s_DetourMap.at("ClientCommand")->GetOriginal<decltype(&ClientCommandHook)>()(clientNum, s);

    // Register when the user pressed the A button
    if (!strcmp(s, "notify +gostand"))
        hasJumped = true;

    // The 'notify -gostand' event means the game started
    if (!strcmp(s, "notify -gostand"))
    {
        // The 'notify -gostand' event also occurs when the A button is released so, to avoid
        // resetting the menu every time the player jumps, we need to make sure the
        // 'notify +gostand' event didn't occur just before.
        // We also allow to initialize the menu if it's not already initialized, even if 'n 42'
        // represents a jump. This is useful when loading the plugin while already in a match,
        // which avoids having to end the match and start a new one for testing in development
        if (!hasJumped || !s_CurrentInstance->InMatch())
        {
            Context::Reset();
            Context::ClientNum = 0;

            s_CurrentInstance->InMatch(true);
            s_CurrentInstance->InitMenu();

            // Disable the unlocalized error messages when printing something in the killfeed
            SpecOpsAlphaMW2::Game::Cbuf_AddText(0, "set loc_warnings 0");
            SpecOpsAlphaMW2::Game::Cbuf_AddText(0, "set loc_warningsUI 0");
        }

        // Register that the user released the A button
        hasJumped = false;
    }
}

void SpecOpsAlphaMW2Title::InitRenderer()
{
    UI::R_AddCmdDrawStretchPic = reinterpret_cast<UI::R_ADDCMDDRAWSTRETCHPIC>(0x82386590);
    UI::R_AddCmdDrawText = reinterpret_cast<UI::R_ADDCMDDRAWTEXT>(0x82386E50);
    UI::R_TextWidth = reinterpret_cast<UI::R_TEXTWIDTH>(0x82380D20);
    UI::R_TextHeight = reinterpret_cast<UI::R_TEXTHEIGHT>(0x82380E10);
    UI::R_RegisterFont = reinterpret_cast<UI::R_REGISTERFONT>(0x82380CB0);
    UI::Material_RegisterHandle = reinterpret_cast<UI::MATERIAL_REGISTERHANDLE>(0x82380880);

    Title::InitRenderer();
}
