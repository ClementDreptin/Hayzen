#include "pch.h"
#include "Games/SpecOps/AlphaMW2/SpecOpsAlphaMW2Title.h"

#include "Core/Context.h"
#include "Core/OptionGroup.h"
#include "Options/ClickOption.h"
#include "Options/ToggleOption.h"
#include "Options/RangeOption.h"
#include "UI/Renderer.h"
#include "Games/SpecOps/AlphaMW2/MenuFunctions.h"
#include "Games/SpecOps/AlphaMW2/GameFunctions.h"

Detour *SpecOpsAlphaMW2Title::s_pClientCommandDetour = nullptr;

SpecOpsAlphaMW2Title::SpecOpsAlphaMW2Title()
{
    Xam::XNotify("Hayzen - MW2 Alpha Spec Ops Detected");

    // Give the system some time to fully load the game in memory
    Sleep(200);

    // Initialize the renderer
    InitRenderer();

    // Set up the function hooks
    s_pSCR_DrawScreenFieldDetour = new Detour(0x82133BE0, SCR_DrawScreenFieldHook);
    s_pClientCommandDetour = new Detour(0x821EA940, ClientCommandHook);
}

SpecOpsAlphaMW2Title::~SpecOpsAlphaMW2Title()
{
    delete s_pClientCommandDetour;
}

void SpecOpsAlphaMW2Title::InitMenu()
{
    std::vector<OptionGroup> optionGroups;

    bool isUnlimitedAmmoEnabled = Memory::Read<POWERPC_INSTRUCTION>(0x82328610) == 0x7D284B78;

    // Main section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ToggleOption, "God Mode", SpecOpsAlphaMW2::ToggleGodMode));
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
        options.emplace_back(MakeOption(ToggleOption, "UFO", SpecOpsAlphaMW2::ToggleUfo));
        optionGroups.emplace_back(OptionGroup("Teleport", options));
    }

    // Second player section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ToggleOption, "God Mode", SpecOpsAlphaMW2::ToggleSecondPlayerGodMode));
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
    // Call the original ClientCommand function
    s_pClientCommandDetour->GetOriginal<decltype(&ClientCommandHook)>()(clientNum, s);

    // Register when the user pressed the A button
    if (!strcmp(s, "notify +gostand"))
        hasJumped = true;

    // The 'n 42' event means the game started
    if (!strcmp(s, "notify -gostand"))
    {
        // The 'n 42' event also occurs when the A button is released so, to avoid
        // resetting the menu every time the player jumps, we need to make sure the
        // 'n 7' event didn't occur just before.
        if (!hasJumped)
        {
            // Reset the context
            Context::Reset();
            Context::ClientNum = 0;

            // Initialize the menu
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
    using namespace Renderer;

    R_AddCmdDrawStretchPic = reinterpret_cast<R_ADDCMDDRAWSTRETCHPIC>(0x82386590);
    R_AddCmdDrawText = reinterpret_cast<R_ADDCMDDRAWTEXT>(0x82386E50);
    R_TextWidth = reinterpret_cast<R_TEXTWIDTH>(0x82380D20);
    R_TextHeight = reinterpret_cast<R_TEXTHEIGHT>(0x82380E10);
    R_RegisterFont = reinterpret_cast<R_REGISTERFONT>(0x82380CB0);
    Material_RegisterHandle = reinterpret_cast<MATERIAL_REGISTERHANDLE>(0x82380880);

    Title::InitRenderer();
}
