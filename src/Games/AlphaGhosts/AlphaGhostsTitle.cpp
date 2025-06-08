#include "pch.h"
#include "Games/AlphaGhosts/AlphaGhostsTitle.h"

#include "Core/Context.h"
#include "Core/UI.h"
#include "Games/AlphaGhosts/GameFunctions.h"
#include "Games/AlphaGhosts/MenuFunctions.h"
#include "Modules/Binds.h"
#include "Options/ClickOption.h"
#include "Options/OptionGroup.h"
#include "Options/RangeOption.h"
#include "Options/SelectOption.h"
#include "Options/SubOptionGroup.h"
#include "Options/ToggleOption.h"

AlphaGhostsTitle::AlphaGhostsTitle()
{
    WaitUntilReady();

    // Bypass whitelist error
    Memory::Write<uint32_t>(0x8268FF08, 0x60000000);

    InitRenderer();

    // Set up the function hooks
    s_DetourMap["SCR_DrawScreenField"] = Detour(0x82486110, SCR_DrawScreenFieldHook);
    s_DetourMap["Scr_Notify"] = Detour(0x8263B778, Scr_NotifyHook);
    s_DetourMap["SV_ExecuteClientCommand"] = Detour(0x82765210, SV_ExecuteClientCommandHook);

    InstallHooks();

    Xam::XNotify("Hayzen - Ghosts Alpha Multiplayer Detected");
}

void AlphaGhostsTitle::InitMenu()
{
    std::vector<OptionGroup> optionGroups;

    // Main section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ToggleOption, "God Mode", AlphaGhosts::ToggleGodMode, false));
        options.emplace_back(MakeOption(ToggleOption, "Remove Invisible Barriers", AlphaGhosts::GoThroughInvisibleBarriers, false));
        optionGroups.emplace_back(OptionGroup("Main", options));
    }

    // Spawn section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ClickOption, "Care Package", AlphaGhosts::SpawnCarePackage));
        options.emplace_back(MakeOption(ClickOption, "Blocker", AlphaGhosts::SpawnBlocker));

        // Spawn settings
        {
            std::vector<std::shared_ptr<Option>> spawnSettings;
            spawnSettings.emplace_back(MakeOption(RangeOption<float>, "Distance", &Context::CarePackageDistance, -10000.0f, 10000.0f, 5.0f));
            spawnSettings.emplace_back(MakeOption(RangeOption<float>, "Height", &Context::CarePackageHeight, -10000.0f, 10000.0f, 5.0f));
            spawnSettings.emplace_back(MakeOption(RangeOption<float>, "Angle", &Context::CarePackageAngle, -90.0f, 90.0f, 1.0f));

            std::vector<std::string> positionPresets(3);
            positionPresets[0] = "Bounce Test";
            positionPresets[1] = "Mega Bounce";
            positionPresets[2] = "Floor";
            spawnSettings.emplace_back(MakeOption(SelectOption, "Position Presets", positionPresets, AlphaGhosts::ChangeCarePackagePositionPresets));

            std::vector<std::string> orientations(3);
            orientations[0] = "Straight";
            orientations[1] = "Left Strafe";
            orientations[2] = "Right Strafe";
            spawnSettings.emplace_back(MakeOption(SelectOption, "Orientiation", orientations, AlphaGhosts::ChangeCarePackageOrientation));

            options.emplace_back(MakeOption(SubOptionGroup, "Settings", spawnSettings));
        }

        optionGroups.emplace_back(OptionGroup("Spawn", options));
    }

    // Teleport section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ToggleOption, "Save/Load Binds", AlphaGhosts::ToggleSaveLoadBinds, false));
        options.emplace_back(MakeOption(ToggleOption, "UFO Bind", AlphaGhosts::ToggleUfoBind, false));
        optionGroups.emplace_back(OptionGroup("Teleport", options));
    }

    // Input Replay
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ToggleOption, "Record Input", AlphaGhosts::RecordInput, false));
        options.emplace_back(MakeOption(ToggleOption, "Replay Input Bind", AlphaGhosts::ToggleReplayInputBind, false));
        optionGroups.emplace_back(OptionGroup("Input", options));
    }

    m_Menu.Init(optionGroups);
}

void AlphaGhostsTitle::Scr_NotifyHook(AlphaGhosts::Game::gentity_s *entity, uint16_t stringValue, uint32_t paramCount)
{
    XASSERT(s_DetourMap.find("Scr_Notify") != s_DetourMap.end());

    // Call the original Scr_Notify function
    s_DetourMap.at("Scr_Notify").GetOriginal<decltype(&Scr_NotifyHook)>()(entity, stringValue, paramCount);

    // If the client is not host, no need to go further
    int clientNum = entity->state.clientNum;
    if (!AlphaGhosts::Game::IsHost(clientNum))
        return;

    // Prevent the menu from initializing in public matches
    if (AlphaGhosts::Game::Dvar_GetBool("xblive_privatematch") == false && AlphaGhosts::Game::Dvar_GetBool("onlinegame") == true)
        return;

    // Get the string representing the event
    const char *eventName = AlphaGhosts::Game::SL_ConvertToString(stringValue);

    // "begin" can happen multiple times a game in round-based gamemodes and we don't want
    // to recreate the menu every round so we make sure we are not already in a match
    // We also allow to initialize the menu on sprint, this is useful when loading the plugin while already in a match,
    // which avoids having to end the match and start a new one for testing in development
    if ((!strcmp(eventName, "begin") || !strcmp(eventName, "sprint_begin")) && !s_CurrentInstance->InMatch())
    {
        Context::Reset();
        Binds::Clear();

        Context::ClientNum = clientNum;

        s_CurrentInstance->InMatch(true);
        s_CurrentInstance->InitMenu();

        // Disable the debug info
        AlphaGhosts::Game::Cbuf_AddText(0, "cg_drawBuildName 0;cg_drawFPS Off;cg_drawVersion 0;cg_drawViewpos 0;con_errorMessageTime 0");
    }
}

void AlphaGhostsTitle::SV_ExecuteClientCommandHook(AlphaGhosts::Game::client_t *client, const char *s, int fromOldServer)
{
    XASSERT(s_DetourMap.find("SV_ExecuteClientCommand") != s_DetourMap.end());

    // Call the original SV_ExecuteClientCommand function
    s_DetourMap.at("SV_ExecuteClientCommand").GetOriginal<decltype(&SV_ExecuteClientCommandHook)>()(client, s, fromOldServer);

    // Stop the menu when the game ends
    if (!strcmp(s, "matchdatadone"))
        s_CurrentInstance->InMatch(false);
}

void AlphaGhostsTitle::InitRenderer()
{
    UI::R_AddCmdDrawStretchPic = reinterpret_cast<UI::R_ADDCMDDRAWSTRETCHPIC>(0x829AF9F8);
    UI::R_AddCmdDrawText = reinterpret_cast<UI::R_ADDCMDDRAWTEXT>(0x829B2118);
    UI::R_TextWidth = reinterpret_cast<UI::R_TEXTWIDTH>(0x8297F380);
    UI::R_TextHeight = reinterpret_cast<UI::R_TEXTHEIGHT>(0x8297F4D8);
    UI::R_RegisterFont = reinterpret_cast<UI::R_REGISTERFONT>(0x8297F280);
    UI::Material_RegisterHandle = reinterpret_cast<UI::MATERIAL_REGISTERHANDLE>(0x82994C78);

    Title::InitRenderer();
}

void AlphaGhostsTitle::WaitUntilReady()
{
    // This variable indicates when the always used assets are ready
    const uintptr_t g_streamAlwaysUsedMemoryAddress = 0x836B9FC8;
    uint32_t ready = Memory::Read<uint32_t>(g_streamAlwaysUsedMemoryAddress);

    while (ready == 0)
    {
        Sleep(1000);
        ready = Memory::Read<uint32_t>(g_streamAlwaysUsedMemoryAddress);
    }
}
