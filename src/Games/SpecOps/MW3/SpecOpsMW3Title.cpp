#include "pch.h"
#include "Games/SpecOps/MW3/SpecOpsMW3Title.h"

#include "Core/Context.h"
#include "Core/OptionGroup.h"
#include "Options/ClickOption.h"
#include "Options/ToggleOption.h"
#include "Options/RangeOption.h"
#include "UI/Renderer.h"
#include "Games/SpecOps/MW3/MenuFunctions.h"

Detour *SpecOpsMW3Title::s_pClientCommandDetour = nullptr;
Detour *SpecOpsMW3Title::s_pPlayerCmd_AllowJumpDetour = nullptr;

SpecOpsMW3Title::SpecOpsMW3Title()
{
    Xam::XNotify("Hayzen - MW3 Spec Ops Detected");

    // Give the system some time to fully load the game in memory
    Sleep(200);

    InitRenderer();

    // Set up the function hooks
    s_pSCR_DrawScreenFieldDetour = new Detour(0x82127090, SCR_DrawScreenFieldHook);
    s_pClientCommandDetour = new Detour(0x821FEFB0, ClientCommandHook);
    s_pPlayerCmd_AllowJumpDetour = new Detour(0x821FA680, PlayerCmd_AllowJumpHook);
}

SpecOpsMW3Title::~SpecOpsMW3Title()
{
    delete s_pClientCommandDetour;
    delete s_pPlayerCmd_AllowJumpDetour;
}

void SpecOpsMW3Title::InitMenu()
{
    std::vector<OptionGroup> optionGroups;

    bool isUnlimitedAmmoEnabled = Memory::Read<POWERPC_INSTRUCTION>(0x8235BB54) == 0x7D495378;

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

static bool hasJumped = false;

void SpecOpsMW3Title::ClientCommandHook(int clientNum, const char *s)
{
    // Call the original ClientCommand function
    s_pClientCommandDetour->GetOriginal<decltype(&ClientCommandHook)>()(clientNum, s);

    // Register when the user pressed the A button
    if (!strcmp(s, "n 25"))
        hasJumped = true;

    // The 'n 26' event means the game started
    if (!strcmp(s, "n 26"))
    {
        // The 'n 26' event also occurs when the A button is released so, to avoid
        // resetting the menu every time the player jumps, we need to make sure the
        // 'n 25' event didn't occur just before.
        if (!hasJumped)
        {
            Context::Reset();
            Context::ClientNum = 0;

            s_CurrentInstance->InMatch(true);
            s_CurrentInstance->InitMenu();
        }

        // Register that the user released the A button
        hasJumped = false;
    }
}

void SpecOpsMW3Title::PlayerCmd_AllowJumpHook()
{
    // Making the PlayerCmd_AllowJump function not do anything so that you can jump in
    // missions where you normally can't. This is a bad practice and may have side effects.
    return;
}

void SpecOpsMW3Title::InitRenderer()
{
    using namespace Renderer;

    R_AddCmdDrawStretchPic = reinterpret_cast<R_ADDCMDDRAWSTRETCHPIC>(0x823F4878);
    R_AddCmdDrawText = reinterpret_cast<R_ADDCMDDRAWTEXT>(0x823F4E30);
    R_TextWidth = reinterpret_cast<R_TEXTWIDTH>(0x823DD318);
    R_TextHeight = reinterpret_cast<R_TEXTHEIGHT>(0x823DD320);
    R_RegisterFont = reinterpret_cast<R_REGISTERFONT>(0x823DD130);
    Material_RegisterHandle = reinterpret_cast<MATERIAL_REGISTERHANDLE>(0x823E95E8);

    Title::InitRenderer();
}
