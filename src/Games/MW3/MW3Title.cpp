#include "pch.h"
#include "Games/MW3/MW3Title.h"

#include "Core/Context.h"
#include "Core/OptionGroup.h"
#include "Options/ClickOption.h"
#include "Options/ToggleOption.h"
#include "UI/Renderer.h"
#include "Games/MW3/MenuFunctions.h"
#include "Games/MW3/GameFunctions.h"

Detour *MW3Title::s_pScr_NotifyDetour = nullptr;
Detour *MW3Title::s_pSV_ExecuteClientCommandDetour = nullptr;

MW3Title::MW3Title()
{
    Xam::XNotify("Hayzen - MW3 Multiplayer Detected");

    // Give the system some time to fully load the game in memory
    Sleep(200);

    // NOP cheat protection
    Memory::Write<int>(0x821ABA24, 0x60000000);
    Memory::Write<int>(0x821ABA9C, 0x60000000);

    // Initialize the renderer
    InitRenderer();

    // Set up the function hooks
    s_pSCR_DrawScreenFieldDetour = new Detour(0x8217CF90, SCR_DrawScreenFieldHook);
    s_pScr_NotifyDetour = new Detour(0x8226AF98, Scr_NotifyHook);
    s_pSV_ExecuteClientCommandDetour = new Detour(0x822C78A0, SV_ExecuteClientCommandHook);
}

MW3Title::~MW3Title()
{
    delete s_pScr_NotifyDetour;
    delete s_pSV_ExecuteClientCommandDetour;
}

void MW3Title::InitMenu()
{
    std::vector<OptionGroup> optionGroups;

    bool isFallDamageEnabled = Memory::Read<float>(0x82000C04) == 999.0f;
    bool isUnlimitedAmmoEnabled = Memory::Read<POWERPC_INSTRUCTION>(0x820F63E4) == 0x7D495378;

    // Main section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ToggleOption, "God Mode", MW3::ToggleGodMode, false));
        options.emplace_back(MakeOption(ToggleOption, "Fall Damage", MW3::ToggleFallDamage, isFallDamageEnabled));
        options.emplace_back(MakeOption(ToggleOption, "Ammo", MW3::ToggleAmmo, isUnlimitedAmmoEnabled));
        options.emplace_back(MakeOption(ClickOption, "Spawn Care Package", MW3::SpawnCarePackage));
        optionGroups.emplace_back(OptionGroup("Main", options));
    }

    // Teleport section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ToggleOption, "Save/Load Binds", MW3::ToggleSaveLoadBinds, &Context::BindsEnabled));
        options.emplace_back(MakeOption(ClickOption, "Save Position", MW3::SavePosition));
        options.emplace_back(MakeOption(ClickOption, "Load Position", MW3::LoadPosition));
        options.emplace_back(MakeOption(ToggleOption, "UFO", MW3::ToggleUfo, false));
        optionGroups.emplace_back(OptionGroup("Teleport", options));
    }

    // Set the save and load functions
    Context::SavePositionFn = MW3::SavePosition;
    Context::LoadPositionFn = MW3::LoadPosition;

    m_Menu.Init(optionGroups);
}

void MW3Title::Scr_NotifyHook(MW3::Game::gentity_s *entity, uint16_t stringValue, uint32_t paramCount)
{
    // Call the original Scr_Notify function
    s_pScr_NotifyDetour->GetOriginal<decltype(&Scr_NotifyHook)>()(entity, stringValue, paramCount);

    // If the client is not host, no need to go further
    int clientNum = entity->state.number;
    if (!MW3::Game::IsHost(clientNum))
        return;

    // Get the string representing the event
    const char *eventName = MW3::Game::SL_ConvertToString(stringValue);

    if (!strcmp(eventName, "begin"))
    {
        // Reset the context
        Context::Reset();
        Context::ClientNum = clientNum;

        // Initialize the menu
        s_CurrentInstance->InMatch(true);
        s_CurrentInstance->InitMenu();

        // Disable the unlocalized error messages when printing something in the killfeed
        MW3::Game::SetClientDvar(clientNum, "loc_warnings", "0");
        MW3::Game::SetClientDvar(clientNum, "loc_warningsUI", "0");
    }
}

void MW3Title::SV_ExecuteClientCommandHook(int client, const char *s, int clientOK, int fromOldServer)
{
    // Call the original Scr_Notify SV_ExecuteClientCommand
    s_pSV_ExecuteClientCommandDetour->GetOriginal<decltype(&SV_ExecuteClientCommandHook)>()(client, s, clientOK, fromOldServer);

    // If the client is not host, no need to go further
    int clientNum = (client - Memory::Read<int>(0x836C6310)) / 0x68B80;
    if (!MW3::Game::IsHost(clientNum))
        return;

    // Stop the menu when the game ends
    if (!strcmp(s, "matchdatadone"))
        s_CurrentInstance->InMatch(false);
}

void MW3Title::InitRenderer()
{
    using namespace Renderer;

    R_AddCmdDrawStretchPic = reinterpret_cast<R_ADDCMDDRAWSTRETCHPIC>(0x8241F288);
    R_AddCmdDrawText = reinterpret_cast<R_ADDCMDDRAWTEXT>(0x8241F9E0);
    R_TextWidth = reinterpret_cast<R_TEXTWIDTH>(0x82407C78);
    R_TextHeight = reinterpret_cast<R_TEXTHEIGHT>(0x82407C80);
    R_RegisterFont = reinterpret_cast<R_REGISTERFONT>(0x82407A90);
    Material_RegisterHandle = reinterpret_cast<MATERIAL_REGISTERHANDLE>(0x82413F48);

    Title::InitRenderer();
}
