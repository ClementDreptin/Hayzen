#include "pch.h"
#include "Games/MW2/MW2Title.h"

#include "Core/Context.h"
#include "Core/UI.h"
#include "Games/MW2/GameFunctions.h"
#include "Games/MW2/MenuFunctions.h"
#include "Modules/Binds.h"
#include "Options/ClickOption.h"
#include "Options/OptionGroup.h"
#include "Options/RangeOption.h"
#include "Options/SelectOption.h"
#include "Options/SubOptionGroup.h"
#include "Options/ToggleOption.h"

MW2Title::MW2Title()
{
    // Give the system some time to fully load the game in memory
    // Devkits are a little slower and need more time
    Sleep(IsDevkit() ? 2000 : 200);

    // NOP cheat protection
    Memory::Write<int>(0x8216906C, 0x60000000);
    Memory::Write<int>(0x821690E4, 0x60000000);

    InitRenderer();

    // Set up the function hooks
    s_DetourMap["SCR_DrawScreenField"] = Detour(0x8214BEB8, SCR_DrawScreenFieldHook);
    s_DetourMap["Scr_Notify"] = Detour(0x82209710, Scr_NotifyHook);
    s_DetourMap["SV_ExecuteClientCommand"] = Detour(0x82253140, SV_ExecuteClientCommandHook);

    InstallHooks();

    Xam::XNotify("Hayzen - MW2 Multiplayer Detected");
}

void MW2Title::InitMenu()
{
    std::vector<OptionGroup> optionGroups;

    bool isFallDamageEnabled = Memory::Read<float>(0x82019C48) == 9999.0f;
    bool isUnlimitedAmmoEnabled = Memory::Read<uint32_t>(0x820E1724) == 0x7D284B78;
    bool goThroughInvisibleBarriersEnabled =
        s_DetourMap.find("PM_CheckLadderMove") != s_DetourMap.end() &&
        s_DetourMap.find("PmoveSingle") != s_DetourMap.end();

    // Main section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ToggleOption, "God Mode", MW2::ToggleGodMode, false));
        options.emplace_back(MakeOption(ToggleOption, "Fall Damage", MW2::ToggleFallDamage, isFallDamageEnabled));
        options.emplace_back(MakeOption(ToggleOption, "Ammo", MW2::ToggleAmmo, isUnlimitedAmmoEnabled));
        options.emplace_back(MakeOption(RangeOption<float>, "Jump Height", reinterpret_cast<float *>(0x82001A34), 0.0f, 999.0f, 1.0f));
        options.emplace_back(MakeOption(ToggleOption, "Remove Invisible Barriers", MW2::GoThroughInvisibleBarriers, goThroughInvisibleBarriersEnabled));
        optionGroups.emplace_back(OptionGroup("Main", options));
    }

    // Spawn section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ClickOption, "Crate", MW2::SpawnCrate));
        options.emplace_back(MakeOption(ClickOption, "Blocker", MW2::SpawnBlocker));

        // Spawn settings
        {
            std::vector<std::shared_ptr<Option>> spawnSettings;
            spawnSettings.emplace_back(MakeOption(RangeOption<float>, "Distance", &Context::CrateDistance, -10000.0f, 10000.0f, 5.0f));
            spawnSettings.emplace_back(MakeOption(RangeOption<float>, "Height", &Context::CrateHeight, -10000.0f, 10000.0f, 5.0f));
            spawnSettings.emplace_back(MakeOption(RangeOption<float>, "Angle", &Context::CrateAngle, -90.0f, 90.0f, 1.0f));

            std::vector<std::string> positionPresets(3);
            positionPresets[0] = "Bounce Test";
            positionPresets[1] = "Mega Bounce";
            positionPresets[2] = "Floor";
            spawnSettings.emplace_back(MakeOption(SelectOption, "Position Presets", positionPresets, MW2::ChangeCratePositionPresets));

            std::vector<std::string> orientations(3);
            orientations[0] = "Straight";
            orientations[1] = "Left Strafe";
            orientations[2] = "Right Strafe";
            spawnSettings.emplace_back(MakeOption(SelectOption, "Orientation", orientations, MW2::ChangeCrateOrientation));

            options.emplace_back(MakeOption(SubOptionGroup, "Settings", spawnSettings));
        }

        optionGroups.emplace_back(OptionGroup("Spawn", options));
    }

    // Teleport section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ToggleOption, "Save/Load Binds", MW2::ToggleSaveLoadBinds, false));
        options.emplace_back(MakeOption(ToggleOption, "UFO Bind", MW2::ToggleUfoBind, false));
        optionGroups.emplace_back(OptionGroup("Teleport", options));
    }

    // Bot section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ClickOption, "Spawn Bot", MW2::SpawnBot));
        options.emplace_back(MakeOption(ClickOption, "Teleport Bot to Me", MW2::TeleportBotToMe));
        options.emplace_back(MakeOption(ToggleOption, "Freeze Bot", MW2::ToggleBotMovement, true));
        options.emplace_back(MakeOption(ToggleOption, "Bot Attack", MW2::ToggleBotAttack, false));
        optionGroups.emplace_back(OptionGroup("Bot", options));
    }

    // Input Replay
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ToggleOption, "Record Input", MW2::RecordInput, false));
        options.emplace_back(MakeOption(ToggleOption, "Replay Input Bind", MW2::ToggleReplayInputBind, false));
        optionGroups.emplace_back(OptionGroup("Input", options));
    }

    m_Menu.Init(optionGroups);
}

void MW2Title::Scr_NotifyHook(MW2::Game::gentity_s *entity, uint16_t stringValue, uint32_t paramCount)
{
    XASSERT(s_DetourMap.find("Scr_Notify") != s_DetourMap.end());

    // Call the original Scr_Notify function
    s_DetourMap.at("Scr_Notify").GetOriginal<decltype(&Scr_NotifyHook)>()(entity, stringValue, paramCount);

    // If the client is not host, no need to go further
    int clientNum = entity->state.number;
    if (!MW2::Game::IsHost(clientNum))
        return;

    // Prevent the menu from initializing in public matches
    if (MW2::Game::Dvar_GetBool("xblive_privatematch") == false && MW2::Game::Dvar_GetBool("onlinegame") == true)
        return;

    // Get the string representing the event
    const char *eventName = MW2::Game::SL_ConvertToString(stringValue);

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
        MW2::Game::SetClientDvar(clientNum, "loc_warnings", "0");
        MW2::Game::SetClientDvar(clientNum, "loc_warningsUI", "0");
    }
}

void MW2Title::SV_ExecuteClientCommandHook(MW2::Game::client_t *client, const char *s, int clientOK, int fromOldServer)
{
    XASSERT(s_DetourMap.find("SV_ExecuteClientCommand") != s_DetourMap.end());

    // Call the original SV_ExecuteClientCommand function
    s_DetourMap.at("SV_ExecuteClientCommand").GetOriginal<decltype(&SV_ExecuteClientCommandHook)>()(client, s, clientOK, fromOldServer);

    // If the client is not host, no need to go further
    int clientNum = client->gentity->state.number;
    if (!MW2::Game::IsHost(clientNum))
        return;

    // Stop the menu when the game ends
    if (!strcmp(s, "disconnect"))
        s_CurrentInstance->InMatch(false);
}

void MW2Title::InitRenderer()
{
    UI::R_AddCmdDrawStretchPic = reinterpret_cast<UI::R_ADDCMDDRAWSTRETCHPIC>(0x821384D8);
    UI::R_AddCmdDrawText = reinterpret_cast<UI::R_ADDCMDDRAWTEXT>(0x82350278);
    UI::R_TextWidth = reinterpret_cast<UI::R_TEXTWIDTH>(0x8234DD20);
    UI::R_TextHeight = reinterpret_cast<UI::R_TEXTHEIGHT>(0x8234DE10);
    UI::R_RegisterFont = reinterpret_cast<UI::R_REGISTERFONT>(0x8234DCB0);
    UI::Material_RegisterHandle = reinterpret_cast<UI::MATERIAL_REGISTERHANDLE>(0x8234E510);

    Title::InitRenderer();
}
