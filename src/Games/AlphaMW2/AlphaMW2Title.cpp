#include "pch.h"
#include "Games/AlphaMW2/AlphaMW2Title.h"

#include "Core/Context.h"
#include "Options/OptionGroup.h"
#include "Options/ClickOption.h"
#include "Options/ToggleOption.h"
#include "Options/RangeOption.h"
#include "UI/Renderer.h"
#include "Games/AlphaMW2/MenuFunctions.h"
#include "Games/AlphaMW2/GameFunctions.h"

AlphaMW2Title::AlphaMW2Title()
{
    // Give the system some time to fully load the game in memory
    // Devkits are a little slower and need more time
    Sleep(Xam::IsDevkit() ? 2000 : 200);

    InitRenderer();

    // Set up the function hooks
    s_DetourMap["SCR_DrawScreenField"] = new Detour(0x8218B5F0, SCR_DrawScreenFieldHook);
    s_DetourMap["Scr_Notify"] = new Detour(0x822539C0, Scr_NotifyHook);
    s_DetourMap["SV_ExecuteClientCommand"] = new Detour(0x822B4700, SV_ExecuteClientCommandHook);

    InstallHooks();

    Xam::XNotify("Hayzen - MW2 Alpha Multiplayer Detected");
}

void AlphaMW2Title::InitMenu()
{
    std::vector<OptionGroup> optionGroups;

    bool isUnlimitedAmmoEnabled = Memory::Read<uint32_t>(0x82113628) == 0x7D284B78;

    // Main section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ToggleOption, "God Mode", AlphaMW2::ToggleGodMode, false));
        options.emplace_back(MakeOption(ToggleOption, "Fall Damage", AlphaMW2::ToggleFallDamage, false));
        options.emplace_back(MakeOption(ToggleOption, "Ammo", AlphaMW2::ToggleAmmo, isUnlimitedAmmoEnabled));
        options.emplace_back(MakeOption(ClickOption, "Spawn Care Package", AlphaMW2::SpawnCarePackage));
        options.emplace_back(MakeOption(ClickOption, "Spawn Blocker", AlphaMW2::SpawnBlocker));
        options.emplace_back(MakeOption(RangeOption<uint32_t>, "Jump Height", AlphaMW2::ChangeJumpHeight, 39, 0, 999, 1));
        optionGroups.emplace_back(OptionGroup("Main", options));
    }

    // Teleport section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ToggleOption, "Save/Load Binds", AlphaMW2::ToggleSaveLoadBinds, &Context::BindsEnabled));
        options.emplace_back(MakeOption(ClickOption, "Save Position", AlphaMW2::SavePosition));
        options.emplace_back(MakeOption(ClickOption, "Load Position", AlphaMW2::LoadPosition));
        options.emplace_back(MakeOption(ToggleOption, "UFO", AlphaMW2::ToggleUfo, false));
        optionGroups.emplace_back(OptionGroup("Teleport", options));
    }

    // Bot section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ClickOption, "Spawn Bot", AlphaMW2::SpawnBot));
        options.emplace_back(MakeOption(ClickOption, "Teleport Bot to Me", AlphaMW2::TeleportBotToMe));
        options.emplace_back(MakeOption(ToggleOption, "Freeze Bot", AlphaMW2::ToggleBotMovement, true));
        optionGroups.emplace_back(OptionGroup("Bot", options));
    }

    // Set the save and load functions
    Context::SavePositionFn = AlphaMW2::SavePosition;
    Context::LoadPositionFn = AlphaMW2::LoadPosition;

    m_Menu.Init(optionGroups);
}

void AlphaMW2Title::Scr_NotifyHook(AlphaMW2::Game::gentity_s *entity, uint16_t stringValue, uint32_t paramCount)
{
    // Call the original Scr_Notify function
    s_DetourMap.at("Scr_Notify")->GetOriginal<decltype(&Scr_NotifyHook)>()(entity, stringValue, paramCount);

    // If the client is not host, no need to go further
    int clientNum = entity->state.number;
    if (!AlphaMW2::Game::IsHost(clientNum))
        return;

    // Get the string representing the event
    const char *eventName = AlphaMW2::Game::SL_ConvertToString(stringValue);

    // "begin" can happen multiple times a game in round-based gamemodes and we don't want
    // to recreate the menu every round so we make sure we are not already in a match
    if (!strcmp(eventName, "begin") && !s_CurrentInstance->InMatch())
    {
        Context::Reset();
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
    // Call the original SV_ExecuteClientCommand function
    s_DetourMap.at("SV_ExecuteClientCommand")->GetOriginal<decltype(&SV_ExecuteClientCommandHook)>()(client, s, clientOK, fromOldServer);

    // If the client is not host, no need to go further
    int clientNum = client->gentity->state.number;
    if (!AlphaMW2::Game::IsHost(clientNum))
        return;

    // Stop the menu when the game ends
    if (!strcmp(s, "matchdatadone"))
        s_CurrentInstance->InMatch(false);
}

void AlphaMW2Title::InitRenderer()
{
    using namespace Renderer;

    R_AddCmdDrawStretchPic = reinterpret_cast<R_ADDCMDDRAWSTRETCHPIC>(0x823BAC18);
    R_AddCmdDrawText = reinterpret_cast<R_ADDCMDDRAWTEXT>(0x823BB4D8);
    R_TextWidth = reinterpret_cast<R_TEXTWIDTH>(0x823B6DC8);
    R_TextHeight = reinterpret_cast<R_TEXTHEIGHT>(0x823B6EB8);
    R_RegisterFont = reinterpret_cast<R_REGISTERFONT>(0x823B6D58);
    Material_RegisterHandle = reinterpret_cast<MATERIAL_REGISTERHANDLE>(0x823B6928);

    Title::InitRenderer();
}
