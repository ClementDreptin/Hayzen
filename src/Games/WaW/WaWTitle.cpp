#include "pch.h"
#include "Games/WaW/WaWTitle.h"

#include "Core/Context.h"
#include "Core/UI.h"
#include "Games/WaW/GameFunctions.h"
#include "Games/WaW/MenuFunctions.h"
#include "Modules/Binds.h"
#include "Options/ClickOption.h"
#include "Options/OptionGroup.h"
#include "Options/RangeOption.h"
#include "Options/SelectOption.h"
#include "Options/SubOptionGroup.h"
#include "Options/ToggleOption.h"

std::string WaWTitle::s_PatchedGameObjectsGscMainFunction;

WaWTitle::WaWTitle()
{
    WaitUntilReady();

    InitRenderer();

    ApplyPatches();

    InstallHooks();

    Xam::XNotify("Hayzen - WaW Multiplayer Detected");
}

void WaWTitle::InitMenu()
{
    std::vector<OptionGroup> optionGroups;

    bool isUnlimitedAmmoEnabled = Memory::Read<uint32_t>(0x82146150) == 0x60000000;
    float jumpHeightValue = WaW::Game::Dvar_GetFloat("jump_height");
    bool goThroughInvisibleBarriersEnabled =
        m_DetourMap.find("PM_CheckLadderMove") != m_DetourMap.end() &&
        m_DetourMap.find("PmoveSingle") != m_DetourMap.end();

    // Main section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ToggleOption, "God Mode", WaW::ToggleGodMode, false));
        options.emplace_back(MakeOption(ToggleOption, "Fall Damage", WaW::ToggleFallDamage, false));
        options.emplace_back(MakeOption(ToggleOption, "Ammo", WaW::ToggleAmmo, isUnlimitedAmmoEnabled));
        options.emplace_back(MakeOption(RangeOption<uint32_t>, "Jump Height", WaW::ChangeJumpHeight, static_cast<uint32_t>(jumpHeightValue), 0, 64, 1));
        options.emplace_back(MakeOption(ToggleOption, "Remove Invisible Barriers", WaW::GoThroughInvisibleBarriers, goThroughInvisibleBarriersEnabled));
        options.emplace_back(MakeOption(ToggleOption, "Uncapped Bounces", WaW::ToggleUncappedBounces, true));
        optionGroups.emplace_back(OptionGroup("Main", options));
    }

    // TODO: fix the mesh offset issue and make the spawn section available in release builds
#ifndef NDEBUG
    // Spawn section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ClickOption, "Crate", WaW::SpawnCrate));

        // This option isn't available on WaW because rotating a brushmodel around the X
        // axis doesn't seem to work, maybe find a fix one day.
        // options.emplace_back(MakeOption(ClickOption, "Blocker", WaW::SpawnBlocker));

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
            spawnSettings.emplace_back(MakeOption(SelectOption, "Position Presets", positionPresets, WaW::ChangeCratePositionPresets));

            std::vector<std::string> orientations(3);
            orientations[0] = "Straight";
            orientations[1] = "Left Strafe";
            orientations[2] = "Right Strafe";
            spawnSettings.emplace_back(MakeOption(SelectOption, "Orientation", orientations, WaW::ChangeCrateOrientation));

            options.emplace_back(MakeOption(SubOptionGroup, "Settings", spawnSettings));
        }

        optionGroups.emplace_back(OptionGroup("Spawn", options));
    }
#endif

    // Teleport section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ToggleOption, "Save/Load Binds", WaW::ToggleSaveLoadBinds, false));
        options.emplace_back(MakeOption(ToggleOption, "UFO Bind", WaW::ToggleUfoBind, false));
        optionGroups.emplace_back(OptionGroup("Teleport", options));
    }

    // Bot section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ClickOption, "Spawn Bot", WaW::SpawnBot));
        options.emplace_back(MakeOption(ClickOption, "Teleport Bot to Me", WaW::TeleportBotToMe));
        options.emplace_back(MakeOption(ToggleOption, "Bot Move", WaW::ToggleBotMovement, false));
        options.emplace_back(MakeOption(ToggleOption, "Bot Attack", WaW::ToggleBotAttack, false));
        optionGroups.emplace_back(OptionGroup("Bot", options));
    }

    // Input Replay
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ToggleOption, "Record Input", WaW::RecordInput, false));
        options.emplace_back(MakeOption(ToggleOption, "Replay Input Bind", WaW::ToggleReplayInputBind, false));
        optionGroups.emplace_back(OptionGroup("Input", options));
    }

    m_Menu = Menu(optionGroups);
}

void WaWTitle::Scr_Notify(WaW::Game::gentity_s *entity, uint16_t stringValue, uint32_t paramCount)
{
    auto &detourMap = Title::GetDetourMap();
    XASSERT(detourMap.find("Scr_Notify") != detourMap.end());

    // Call the original Scr_Notify function
    detourMap.at("Scr_Notify").GetOriginal<decltype(&Scr_Notify)>()(entity, stringValue, paramCount);

    // If the client is not host, no need to go further
    int clientNum = entity->state.number;
    if (!WaW::Game::IsHost(clientNum))
        return;

    // Prevent the menu from initializing in public matches
    if (WaW::Game::Dvar_GetBool("xblive_privatematch") == false && WaW::Game::Dvar_GetBool("onlinegame") == true)
        return;

    // Get the string representing the event
    const char *eventName = WaW::Game::SL_ConvertToString(stringValue, 0);

    // "begin" can happen multiple times a game in round-based gamemodes and we don't want
    // to recreate the menu every round so we make sure we are not already in a match
    // We also allow to initialize the menu on sprint, this is useful when loading the plugin while already in a match,
    // which avoids having to end the match and start a new one for testing in development
    if ((!strcmp(eventName, "begin") || !strcmp(eventName, "sprint_begin")) && !s_CurrentInstance->InMatch())
    {
        Context::Reset();
        Binds::Clear();

        Context::ClientNum = clientNum;

        // Uncap bounces by default
        WaW::Game::SetClientDvar(clientNum, "player_bayonetLaunchProof", "0");

        s_CurrentInstance->InMatch(true);
        s_CurrentInstance->InitMenu();

        // Disable the unlocalized error messages when printing something in the killfeed
        WaW::Game::SetClientDvar(clientNum, "loc_warnings", "0");
        WaW::Game::SetClientDvar(clientNum, "loc_warningsUI", "0");
    }
}

void WaWTitle::G_ShutdownGameHook(bool freeScripts)
{
    auto &detourMap = Title::GetDetourMap();
    XASSERT(detourMap.find("G_ShutdownGame") != detourMap.end());

    // Call the original G_ShutdownGame function
    detourMap.at("G_ShutdownGame").GetOriginal<decltype(&G_ShutdownGameHook)>()(freeScripts);

    // Stop the menu when the game ends
    // freeScripts is true when going back to the lobby and it's false when a round ends
    // in a round based gamemode
    if (freeScripts)
        s_CurrentInstance->InMatch(false);
}

char *WaWTitle::Scr_AddSourceBufferHook(uint32_t scriptInstance, const char *filename, const char *extFilename, const char *codePos, bool archive)
{
    auto &detourMap = Title::GetDetourMap();
    XASSERT(detourMap.find("Scr_AddSourceBuffer") != detourMap.end());

    // Call the original Scr_AddSourceBuffer function
    char *result = detourMap.at("Scr_AddSourceBuffer").GetOriginal<decltype(&Scr_AddSourceBufferHook)>()(scriptInstance, filename, extFilename, codePos, archive);

    // Modify _gameobjects.gsc
    if (!strcmp(extFilename, "maps/mp/gametypes/_gameobjects.gsc"))
    {
        const size_t originalMainFunctionStartOffset = 0x55;
        XASSERT(strlen(result) > originalMainFunctionStartOffset);

        // Replace the beginning of the script (the start of its main function) with our patched version
        s_PatchedGameObjectsGscMainFunction += &result[originalMainFunctionStartOffset];

        return const_cast<char *>(s_PatchedGameObjectsGscMainFunction.c_str());
    }

    return result;
}

void WaWTitle::ApplyPatches()
{
    // We need to patch a GSC script to make the HQ tables spawn in every gamemode
    s_PatchedGameObjectsGscMainFunction =
        "#include maps\\mp\\_utility;\n"
        "#include maps\\mp\\gametypes\\_hud_util;\n"
        "\n"
        "main(allowed)\n"
        "{\n"
        "	// PATCH: By appending 'hq' to the list of allowed gamemodes, we always allow HQ tables to spawn\n"
        "	allowed[allowed.size] = \"hq\";\n";

    m_DetourMap["Scr_AddSourceBuffer"] = Detour(0x82339EF8, Scr_AddSourceBufferHook);
}

void WaWTitle::InstallHooks()
{
    m_DetourMap["SCR_DrawScreenField"] = Detour(0x821BF188, SCR_DrawScreenFieldHook);
    m_DetourMap["Scr_Notify"] = Detour(0x82254180, Scr_Notify);
    m_DetourMap["G_ShutdownGame"] = Detour(0x82220C80, G_ShutdownGameHook);

    Title::InstallHooks();
}

void WaWTitle::InitRenderer()
{
    UI::R_AddCmdDrawStretchPic = reinterpret_cast<UI::R_ADDCMDDRAWSTRETCHPIC>(0x82401410);
    UI::R_AddCmdDrawText = reinterpret_cast<UI::R_ADDCMDDRAWTEXT>(0x82401C30);
    UI::R_TextWidth = reinterpret_cast<UI::R_TEXTWIDTH>(0x823FFF28);
    UI::R_TextHeight = reinterpret_cast<UI::R_TEXTHEIGHT>(0x82400018);
    UI::R_RegisterFont = reinterpret_cast<UI::R_REGISTERFONT>(0x823FFEE0);
    UI::Material_RegisterHandle = reinterpret_cast<UI::MATERIAL_REGISTERHANDLE>(0x824006F0);

    Title::InitRenderer();
}
