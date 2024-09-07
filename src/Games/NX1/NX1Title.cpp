#include "pch.h"
#include "Games/NX1/NX1Title.h"

#include "Core/Context.h"
#include "Options/OptionGroup.h"
#include "Options/ClickOption.h"
#include "Options/ToggleOption.h"
#include "Options/RangeOption.h"
#include "UI/Renderer.h"
#include "Games/NX1/MenuFunctions.h"
#include "Games/NX1/GameFunctions.h"

NX1Title::NX1Title()
{
    WaitUntilReady();

    InitRenderer();

    // Set up the function hooks
    s_DetourMap["SCR_DrawScreenField"] = new Detour(0x821AAAA0, SCR_DrawScreenFieldHook);
    s_DetourMap["Scr_Notify"] = new Detour(0x822AADE0, Scr_NotifyHook);
    s_DetourMap["SV_ExecuteClientCommand"] = new Detour(0x8231B8E8, SV_ExecuteClientCommandHook);

    InstallHooks();

    Xam::XNotify("Hayzen - NX1 Multiplayer Detected");
}

void NX1Title::InitMenu()
{
    std::vector<OptionGroup> optionGroups;

    // Main section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ToggleOption, "God Mode", NX1::ToggleGodMode, false));
        options.emplace_back(MakeOption(ClickOption, "Spawn Care Package", NX1::SpawnCarePackage));
        optionGroups.emplace_back(OptionGroup("Main", options));
    }

    // Teleport section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ToggleOption, "Save/Load Binds", NX1::ToggleSaveLoadBinds, &Context::BindsEnabled));
        options.emplace_back(MakeOption(ClickOption, "Save Position", NX1::SavePosition));
        options.emplace_back(MakeOption(ClickOption, "Load Position", NX1::LoadPosition));
        options.emplace_back(MakeOption(ToggleOption, "UFO", NX1::ToggleUfo, false));
        optionGroups.emplace_back(OptionGroup("Teleport", options));
    }

    // Bot section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ClickOption, "Spawn Bot", NX1::SpawnBot));
        options.emplace_back(MakeOption(ClickOption, "Teleport Bot to Me", NX1::TeleportBotToMe));
        options.emplace_back(MakeOption(ToggleOption, "Freeze Bot", NX1::ToggleBotMovement, true));
        optionGroups.emplace_back(OptionGroup("Bot", options));
    }

    // Set the save and load functions
    Context::SavePositionFn = NX1::SavePosition;
    Context::LoadPositionFn = NX1::LoadPosition;

    m_Menu.Init(optionGroups);
}

void NX1Title::Scr_NotifyHook(NX1::Game::gentity_s *entity, uint16_t stringValue, uint32_t paramCount)
{
    // Call the original Scr_Notify function
    s_DetourMap.at("Scr_Notify")->GetOriginal<decltype(&Scr_NotifyHook)>()(entity, stringValue, paramCount);

    // If the client is not host, no need to go further
    int clientNum = entity->state.number;
    if (!NX1::Game::IsHost(clientNum))
        return;

    // Get the string representing the event
    const char *eventName = NX1::Game::SL_ConvertToString(stringValue);

    // "begin" can happen multiple times a game in round-based gamemodes and we don't want
    // to recreate the menu every round so we make sure we are not already in a match
    if (!strcmp(eventName, "begin") && !s_CurrentInstance->InMatch())
    {
        Context::Reset();
        Context::ClientNum = clientNum;

        s_CurrentInstance->InMatch(true);
        s_CurrentInstance->InitMenu();

        // Disable the unlocalized error messages when printing something in the killfeed
        NX1::Game::SetClientDvar(clientNum, "loc_warnings", "0");
        NX1::Game::SetClientDvar(clientNum, "loc_warningsUI", "0");

        // Disable the debug info
        NX1::Game::SetClientDvar(clientNum, "cg_drawImageCache", "Off");
        NX1::Game::SetClientDvar(clientNum, "cg_drawFPS", "Off");
        NX1::Game::SetClientDvar(clientNum, "cg_drawViewpos", "0");
    }
}

void NX1Title::SV_ExecuteClientCommandHook(NX1::Game::client_t *client, const char *s, int clientOK, int fromOldServer)
{
    // Call the original SV_ExecuteClientCommand function
    s_DetourMap.at("SV_ExecuteClientCommand")->GetOriginal<decltype(&SV_ExecuteClientCommandHook)>()(client, s, clientOK, fromOldServer);

    // If the client is not host, no need to go further
    int clientNum = client->gentity->state.number;
    if (!NX1::Game::IsHost(clientNum))
        return;

    // Stop the menu when the game ends
    if (!strcmp(s, "matchdatadone"))
        s_CurrentInstance->InMatch(false);
}

void NX1Title::InitRenderer()
{
    using namespace Renderer;

    R_AddCmdDrawStretchPic = reinterpret_cast<R_ADDCMDDRAWSTRETCHPIC>(0x824CB298);
    R_AddCmdDrawText = reinterpret_cast<R_ADDCMDDRAWTEXT>(0x824CBB00);
    R_TextWidth = reinterpret_cast<R_TEXTWIDTH>(0x824C9230);
    R_TextHeight = reinterpret_cast<R_TEXTHEIGHT>(0x824C93A0);
    R_RegisterFont = reinterpret_cast<R_REGISTERFONT>(0x824C91C0);
    Material_RegisterHandle = reinterpret_cast<MATERIAL_REGISTERHANDLE>(0x824C9AF8);

    Title::InitRenderer();
}

void NX1Title::WaitUntilReady()
{
    // This variable indicates when the always loaded assets are ready
    const uintptr_t g_streamAlwaysLoadedMemoryAddress = 0x82FA2E80;
    uint32_t ready = Memory::Read<uint32_t>(g_streamAlwaysLoadedMemoryAddress);

    while (ready == 0)
    {
        Sleep(1000);
        ready = Memory::Read<uint32_t>(g_streamAlwaysLoadedMemoryAddress);
    }
}
