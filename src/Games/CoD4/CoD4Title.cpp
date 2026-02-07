#include "pch.h"
#include "Games/CoD4/CoD4Title.h"

#include "Core/Context.h"
#include "Core/UI.h"
#include "Games/CoD4/GameFunctions.h"
#include "Games/CoD4/MenuFunctions.h"
#include "Modules/Binds.h"
#include "Options/ClickOption.h"
#include "Options/OptionGroup.h"
#include "Options/RangeOption.h"
#include "Options/SelectOption.h"
#include "Options/SubOptionGroup.h"
#include "Options/ToggleOption.h"

std::string CoD4Title::s_PatchedGameObjectsGscMainFunction;

CoD4Title::CoD4Title()
{
    // Give the system some time to fully load the game in memory
    // Devkits are a little slower and need more time
    Sleep(IsDevkit() ? 2000 : 200);

    InitRenderer();

    // Set up the function hooks
    s_DetourMap["SCR_DrawScreenField"] = Detour(0x822C9D50, SCR_DrawScreenFieldHook);
    s_DetourMap["Scr_NotifyNum"] = Detour(0x82217890, Scr_NotifyNumHook);
    s_DetourMap["G_ShutdownGame"] = Detour(0x82272E58, G_ShutdownGameHook);

    ApplyPatches();

    InstallHooks();

    Xam::XNotify("Hayzen - CoD4 Multiplayer Detected");
}

void CoD4Title::InitMenu()
{
    std::vector<OptionGroup> optionGroups;

    bool isUnlimitedAmmoEnabled = Memory::Read<uint32_t>(0x82332088) == 0x60000000;
    float jumpHeightValue = CoD4::Game::Dvar_GetFloat("jump_height");
    bool goThroughInvisibleBarriersEnabled =
        s_DetourMap.find("PM_CheckLadderMove") != s_DetourMap.end() &&
        s_DetourMap.find("PmoveSingle") != s_DetourMap.end();

    // Main section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ToggleOption, "God Mode", CoD4::ToggleGodMode, false));
        options.emplace_back(MakeOption(ToggleOption, "Fall Damage", CoD4::ToggleFallDamage, false));
        options.emplace_back(MakeOption(ToggleOption, "Ammo", CoD4::ToggleAmmo, isUnlimitedAmmoEnabled));
        options.emplace_back(MakeOption(RangeOption<uint32_t>, "Jump Height", CoD4::ChangeJumpHeight, static_cast<uint32_t>(jumpHeightValue), 0, 999, 1));
        options.emplace_back(MakeOption(ToggleOption, "Remove Invisible Barriers", CoD4::GoThroughInvisibleBarriers, goThroughInvisibleBarriersEnabled));
        optionGroups.emplace_back(OptionGroup("Main", options));
    }

    // Spawn section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ClickOption, "Care Package", CoD4::SpawnCarePackage));

        // This option isn't available on CoD4 because rotating a brushmodel around the X
        // axis doesn't seem to work, maybe find a fix one day.
        // options.emplace_back(MakeOption(ClickOption, "Blocker", CoD4::SpawnBlocker));

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
            spawnSettings.emplace_back(MakeOption(SelectOption, "Position Presets", positionPresets, CoD4::ChangeCarePackagePositionPresets));

            std::vector<std::string> orientations(3);
            orientations[0] = "Straight";
            orientations[1] = "Left Strafe";
            orientations[2] = "Right Strafe";
            spawnSettings.emplace_back(MakeOption(SelectOption, "Orientation", orientations, CoD4::ChangeCarePackageOrientation));

            options.emplace_back(MakeOption(SubOptionGroup, "Settings", spawnSettings));
        }

        optionGroups.emplace_back(OptionGroup("Spawn", options));
    }

    // Teleport section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ToggleOption, "Save/Load Binds", CoD4::ToggleSaveLoadBinds, false));
        options.emplace_back(MakeOption(ToggleOption, "UFO Bind", CoD4::ToggleUfoBind, false));
        optionGroups.emplace_back(OptionGroup("Teleport", options));
    }

    // Bot section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ClickOption, "Spawn Bot", CoD4::SpawnBot));
        options.emplace_back(MakeOption(ClickOption, "Teleport Bot to Me", CoD4::TeleportBotToMe));
        options.emplace_back(MakeOption(ToggleOption, "Freeze Bot", CoD4::ToggleBotMovement, true));
        optionGroups.emplace_back(OptionGroup("Bot", options));
    }

    // Input Replay
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ToggleOption, "Record Input", CoD4::RecordInput, false));
        options.emplace_back(MakeOption(ToggleOption, "Replay Input Bind", CoD4::ToggleReplayInputBind, false));
        optionGroups.emplace_back(OptionGroup("Input", options));
    }

    m_Menu.Init(optionGroups);
}

void CoD4Title::Scr_NotifyNumHook(int entNum, uint32_t classNum, uint32_t stringValue, uint32_t paramCount)
{
    XASSERT(s_DetourMap.find("Scr_NotifyNum") != s_DetourMap.end());

    // Call the original Scr_Notify function
    s_DetourMap.at("Scr_NotifyNum").GetOriginal<decltype(&Scr_NotifyNumHook)>()(entNum, classNum, stringValue, paramCount);

    // If the client is not host, no need to go further
    int clientNum = entNum;
    if (!CoD4::Game::IsHost(clientNum))
        return;

    // Prevent the menu from initializing in public matches
    if (CoD4::Game::Dvar_GetBool("xblive_privatematch") == false && CoD4::Game::Dvar_GetBool("onlinegame") == true)
        return;

    // Get the string representing the event
    const char *eventName = CoD4::Game::SL_ConvertToString(stringValue);

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
        CoD4::Game::SetClientDvar(clientNum, "loc_warnings", "0");
        CoD4::Game::SetClientDvar(clientNum, "loc_warningsUI", "0");
    }
}

void CoD4Title::G_ShutdownGameHook(bool freeScripts)
{
    // Call the original Scr_ShutdownSystem function
    s_DetourMap.at("G_ShutdownGame").GetOriginal<decltype(&G_ShutdownGameHook)>()(freeScripts);

    // Stop the menu when the game ends
    // freeScripts is true when going back to the lobby and it's false when a round ends
    // in a round based gamemode
    if (freeScripts)
        s_CurrentInstance->InMatch(false);
}

void CoD4Title::InitRenderer()
{
    UI::R_AddCmdDrawStretchPic = reinterpret_cast<UI::R_ADDCMDDRAWSTRETCHPIC>(0x8216BAE8);
    UI::R_AddCmdDrawText = reinterpret_cast<UI::R_ADDCMDDRAWTEXT>(0x8216C0B8);
    UI::R_TextWidth = reinterpret_cast<UI::R_TEXTWIDTH>(0x8216EB00);
    UI::R_TextHeight = CoD4::Game::R_TextHeight;
    UI::R_RegisterFont = CoD4::Game::R_RegisterFont;
    UI::Material_RegisterHandle = CoD4::Game::Material_RegisterHandle;

    Title::InitRenderer();
}

void CoD4Title::ApplyPatches()
{
    // Dvar_GetFloat(const char *dvarName) is only called with "m_pitch" in CoD4 so the
    // compiler removed the argument and added an instruction to always put "m_pitch" into
    // the r3 register. We nop this instruction to keep the argument into the r3 register
    // and "undo" the compiler optimization
    Memory::Write<uint32_t>(0x821D1508, 0x60000000);

    // We need to patch a GSC script to make the HQ crates spawn in every gamemode
    s_PatchedGameObjectsGscMainFunction =
        "#include maps\\mp\\_utility;"
        "#include maps\\mp\\gametypes\\_hud_util;"
        ""
        "main(allowed)"
        "{"
        "	entitytypes = getentarray();"
        "	for(i = 0; i < entitytypes.size; i++)"
        "	{"
        "		if(isdefined(entitytypes[i].script_gameobjectname))"
        "		{"
        "			dodelete = true;"
        ""
        "			// allow a space-separated list of gameobjectnames\n"
        "			gameobjectnames = strtok(entitytypes[i].script_gameobjectname, \" \");"
        ""
        "			for(j = 0; j < allowed.size; j++)"
        "			{"
        "				for (k = 0; k < gameobjectnames.size; k++)"
        "				{"
        "					// PATCH: we also allow HQ crates to spawn no matter what\n"
        "					if(gameobjectnames[k] == allowed[j] || gameobjectnames[k] == \"hq\")"
        "					{"
        "						dodelete = false;"
        "						break;"
        "					}"
        "				}"
        "				if (!dodelete)"
        "					break;"
        "			}"
        ""
        "			if(dodelete)"
        "			{"
        "				//println(\"DELETED: \", entitytypes[i].classname);\n"
        "				entitytypes[i] delete();"
        "			}"
        "		}"
        "	}"
        "}";

    s_DetourMap["Scr_AddSourceBuffer"] = Detour(0x822212C0, Scr_AddSourceBufferHook);
}

char *CoD4Title::Scr_AddSourceBufferHook(const char *filename, const char *extFilename, const char *codePos, bool archive)
{
    char *result = s_DetourMap.at("Scr_AddSourceBuffer").GetOriginal<decltype(&Scr_AddSourceBufferHook)>()(filename, extFilename, codePos, archive);

    if (!strcmp(extFilename, "maps/mp/gametypes/_gameobjects.gsc"))
    {
        const size_t originalMainFunctionEndOffset = 0x2F2;
        XASSERT(strlen(result) > originalMainFunctionEndOffset);

        // Replace the beginning of the script (its main function) with our patched version
        s_PatchedGameObjectsGscMainFunction += &result[originalMainFunctionEndOffset];

        return const_cast<char *>(s_PatchedGameObjectsGscMainFunction.c_str());
    }

    return result;
}
