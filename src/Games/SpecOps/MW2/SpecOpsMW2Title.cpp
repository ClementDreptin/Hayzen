#include "pch.h"
#include "Games/SpecOps/MW2/SpecOpsMW2Title.h"

#include "Core/Context.h"
#include "Core/OptionGroup.h"
#include "Options/ClickOption.h"
#include "Options/ToggleOption.h"
#include "Options/RangeOption.h"
#include "UI/Renderer.h"
#include "Games/SpecOps/MW2/MenuFunctions.h"

Detour *SpecOpsMW2Title::s_pClientCommandDetour = nullptr;

SpecOpsMW2Title::SpecOpsMW2Title()
{
    Xam::XNotify("Hayzen - MW2 Spec Ops Detected");

    // Give the system some time to fully load the game in memory
    Sleep(200);

    // Initialize the renderer
    InitRenderer();

    // Set up the function hooks
    s_pSCR_DrawScreenFieldDetour = new Detour(0x821354B0, SCR_DrawScreenFieldHook);
    s_pClientCommandDetour = new Detour(0x821EFFD0, ClientCommandHook);
}

SpecOpsMW2Title::~SpecOpsMW2Title()
{
    delete s_pClientCommandDetour;
}

void SpecOpsMW2Title::InitMenu()
{
    std::vector<OptionGroup> optionGroups;

    // Check if the unlimited ammo patch address if equal to the patched value
    bool isUnlimitedAmmoEnabled = Memory::Read<POWERPC_INSTRUCTION>(0x82331F48) == 0x7D284B78;

    // Main section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ToggleOption, "God Mode", SpecOpsMW2::ToggleGodMode));
        options.emplace_back(MakeOption(ToggleOption, "Ammo", SpecOpsMW2::ToggleAmmo, isUnlimitedAmmoEnabled));
        options.emplace_back(MakeOption(RangeOption<uint32_t>, "Jump Height", SpecOpsMW2::ChangeJumpHeight, 39, 0, 999, 1));
        optionGroups.emplace_back(OptionGroup("Main", options));
    }

    // Set the save and load functions
    // Context::SavePositionFn = SpecOpsMW2::SavePosition;
    // Context::LoadPositionFn = SpecOpsMW2::LoadPosition;

    m_Menu.Init(optionGroups);
}

static bool hasJumped = false;

void SpecOpsMW2Title::ClientCommandHook(int clientNum, const char *s)
{
    // Call the original ClientCommand function
    s_pClientCommandDetour->GetOriginal<decltype(&ClientCommandHook)>()(clientNum, s);

    // Register when the user pressed the A button
    if (!strcmp(s, "n 7"))
        hasJumped = true;

    // The 'n 42' event means the game started
    if (!strcmp(s, "n 42"))
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
            SpecOpsMW2::Game::Cbuf_AddText(0, "set loc_warnings 0");
            SpecOpsMW2::Game::Cbuf_AddText(0, "set loc_warningsUI 0");
        }

        // Register that the user released the A button
        hasJumped = false;
    }
}

void SpecOpsMW2Title::InitRenderer()
{
    using namespace Renderer;

    R_AddCmdDrawStretchPic = reinterpret_cast<R_ADDCMDDRAWSTRETCHPIC>(0x82391620);
    R_AddCmdDrawText = reinterpret_cast<R_ADDCMDDRAWTEXT>(0x82391F00);
    R_TextWidth = reinterpret_cast<R_TEXTWIDTH>(0x8238B618);
    R_TextHeight = reinterpret_cast<R_TEXTHEIGHT>(0x8238B708);
    R_RegisterFont = reinterpret_cast<R_REGISTERFONT>(0x8238B5A8);
    Material_RegisterHandle = reinterpret_cast<MATERIAL_REGISTERHANDLE>(0x8238BE08);

    Title::InitRenderer();
}
