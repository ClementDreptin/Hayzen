#include "pch.h"
#include "Games/AlphaMW2/AlphaMW2Title.h"

#include "Core/Context.h"
#include "Core/UI.h"
#include "Games/AlphaMW2/GameFunctions.h"
#include "Games/AlphaMW2/MenuFunctions.h"
#include "Modules/Binds.h"
#include "Options/ClickOption.h"
#include "Options/OptionGroup.h"
#include "Options/RangeOption.h"
#include "Options/SelectOption.h"
#include "Options/SubOptionGroup.h"
#include "Options/ToggleOption.h"

AlphaMW2Title::AlphaMW2Title()
    : m_Console(Console::Props(AlphaMW2::Game::Cbuf_AddText, AlphaMW2::Game::Dvar_ForEach))
{
    // Give the system some time to fully load the game in memory
    // Devkits are a little slower and need more time
    Sleep(IsDevkit() ? 2000 : 200);

    InitRenderer();

    // Set up the function hooks
    s_DetourMap["SCR_DrawScreenField"] = Detour(0x8218B5F0, SCR_DrawScreenFieldHook);
    s_DetourMap["Scr_Notify"] = Detour(0x822539C0, Scr_NotifyHook);
    s_DetourMap["SV_ExecuteClientCommand"] = Detour(0x822B4700, SV_ExecuteClientCommandHook);

    InstallHooks();

    Xam::XNotify("Hayzen - MW2 Alpha Multiplayer Detected");
}

void AlphaMW2Title::InitMenu()
{
    std::vector<OptionGroup> optionGroups;

    bool isUnlimitedAmmoEnabled = Memory::Read<uint32_t>(0x82113628) == 0x7D284B78;
    float jumpHeightValue = AlphaMW2::Game::Dvar_GetFloat("jump_height");
    bool goThroughInvisibleBarriersEnabled =
        s_DetourMap.find("PM_CheckLadderMove") != s_DetourMap.end() &&
        s_DetourMap.find("PmoveSingle") != s_DetourMap.end();

    // Main section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ToggleOption, "God Mode", AlphaMW2::ToggleGodMode, false));
        options.emplace_back(MakeOption(ToggleOption, "Fall Damage", AlphaMW2::ToggleFallDamage, false));
        options.emplace_back(MakeOption(ToggleOption, "Ammo", AlphaMW2::ToggleAmmo, isUnlimitedAmmoEnabled));
        options.emplace_back(MakeOption(RangeOption<uint32_t>, "Jump Height", AlphaMW2::ChangeJumpHeight, static_cast<uint32_t>(jumpHeightValue), 0, 999, 1));
        options.emplace_back(MakeOption(ToggleOption, "Remove Invisible Barriers", AlphaMW2::GoThroughInvisibleBarriers, goThroughInvisibleBarriersEnabled));
        optionGroups.emplace_back(OptionGroup("Main", options));
    }

    // Spawn section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ClickOption, "Care Package", AlphaMW2::SpawnCarePackage));
        options.emplace_back(MakeOption(ClickOption, "Blocker", AlphaMW2::SpawnBlocker));

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
            spawnSettings.emplace_back(MakeOption(SelectOption, "Position Presets", positionPresets, AlphaMW2::ChangeCarePackagePositionPresets));

            std::vector<std::string> orientations(3);
            orientations[0] = "Straight";
            orientations[1] = "Left Strafe";
            orientations[2] = "Right Strafe";
            spawnSettings.emplace_back(MakeOption(SelectOption, "Orientiation", orientations, AlphaMW2::ChangeCarePackageOrientation));

            options.emplace_back(MakeOption(SubOptionGroup, "Settings", spawnSettings));
        }

        optionGroups.emplace_back(OptionGroup("Spawn", options));
    }

    // Teleport section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ToggleOption, "Save/Load Binds", AlphaMW2::ToggleSaveLoadBinds, false));
        options.emplace_back(MakeOption(ToggleOption, "UFO Bind", AlphaMW2::ToggleUfoBind, false));
        optionGroups.emplace_back(OptionGroup("Teleport", options));
    }

    // Bot section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ClickOption, "Spawn Bot", AlphaMW2::SpawnBot));
        options.emplace_back(MakeOption(ClickOption, "Teleport Bot to Me", AlphaMW2::TeleportBotToMe));
        options.emplace_back(MakeOption(ToggleOption, "Freeze Bot", AlphaMW2::ToggleBotMovement, true));
        options.emplace_back(MakeOption(ToggleOption, "Bot Attack", AlphaMW2::ToggleBotAttack, false));
        optionGroups.emplace_back(OptionGroup("Bot", options));
    }

    // Input Replay
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ToggleOption, "Record Input", AlphaMW2::RecordInput, false));
        options.emplace_back(MakeOption(ToggleOption, "Replay Input Bind", AlphaMW2::ToggleReplayInputBind, false));
        optionGroups.emplace_back(OptionGroup("Input", options));
    }

    m_Menu.Init(optionGroups);
}

void AlphaMW2Title::Scr_NotifyHook(AlphaMW2::Game::gentity_s *entity, uint16_t stringValue, uint32_t paramCount)
{
    XASSERT(s_DetourMap.find("Scr_Notify") != s_DetourMap.end());

    // Call the original Scr_Notify function
    s_DetourMap.at("Scr_Notify").GetOriginal<decltype(&Scr_NotifyHook)>()(entity, stringValue, paramCount);

    // If the client is not host, no need to go further
    int clientNum = entity->state.number;
    if (!AlphaMW2::Game::IsHost(clientNum))
        return;

    // Get the string representing the event
    const char *eventName = AlphaMW2::Game::SL_ConvertToString(stringValue);

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

        // Disable the unlocalized error messages when printing something in the killfeed
        AlphaMW2::Game::SetClientDvar(clientNum, "loc_warnings", "0");
        AlphaMW2::Game::SetClientDvar(clientNum, "loc_warningsUI", "0");
    }
}

void AlphaMW2Title::SV_ExecuteClientCommandHook(AlphaMW2::Game::client_t *client, const char *s, int clientOK, int fromOldServer)
{
    XASSERT(s_DetourMap.find("SV_ExecuteClientCommand") != s_DetourMap.end());

    // Call the original SV_ExecuteClientCommand function
    s_DetourMap.at("SV_ExecuteClientCommand").GetOriginal<decltype(&SV_ExecuteClientCommandHook)>()(client, s, clientOK, fromOldServer);

    // If the client is not host, no need to go further
    int clientNum = client->gentity->state.number;
    if (!AlphaMW2::Game::IsHost(clientNum))
        return;

    // Stop the menu when the game ends
    if (!strcmp(s, "matchdatadone"))
        s_CurrentInstance->InMatch(false);
}

void AlphaMW2Title::Update()
{
    // Call the parent to update the menu
    Title::Update();

    m_Console.Update();
}

void AlphaMW2Title::Render()
{
    // Call the parent to render the menu
    Title::Render();

    m_Console.Render();
}

void AlphaMW2Title::InitRenderer()
{
    UI::R_AddCmdDrawStretchPic = reinterpret_cast<UI::R_ADDCMDDRAWSTRETCHPIC>(0x823BAC18);
    UI::R_AddCmdDrawText = reinterpret_cast<UI::R_ADDCMDDRAWTEXT>(0x823BB4D8);
    UI::R_TextWidth = reinterpret_cast<UI::R_TEXTWIDTH>(0x823B6DC8);
    UI::R_TextHeight = reinterpret_cast<UI::R_TEXTHEIGHT>(0x823B6EB8);
    UI::R_RegisterFont = reinterpret_cast<UI::R_REGISTERFONT>(0x823B6D58);
    UI::Material_RegisterHandle = reinterpret_cast<UI::MATERIAL_REGISTERHANDLE>(0x823B6928);

    Title::InitRenderer();
}
