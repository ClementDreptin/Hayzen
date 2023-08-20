#include "pch.h"
#include "Games/MW2/MW2Title.h"

#include "Core/Context.h"
#include "Options/OptionGroup.h"
#include "Options/ClickOption.h"
#include "Options/ToggleOption.h"
#include "Options/RangeOption.h"
#include "UI/Renderer.h"
#include "Games/MW2/MenuFunctions.h"
#include "Games/MW2/GameFunctions.h"

MW2Title::MW2Title()
{
    Xam::XNotify("Hayzen - MW2 Multiplayer Detected");

    // Give the system some time to fully load the game in memory
    Sleep(200);

    // NOP cheat protection
    Memory::Write<int>(0x8216906C, 0x60000000);
    Memory::Write<int>(0x821690E4, 0x60000000);

    InitRenderer();

    // Set up the function hooks
    s_DetourMap["SCR_DrawScreenField"] = new Detour(0x8214BEB8, SCR_DrawScreenFieldHook);
    s_DetourMap["Scr_Notify"] = new Detour(0x82209710, Scr_NotifyHook);
    s_DetourMap["SV_ExecuteClientCommand"] = new Detour(0x82253140, SV_ExecuteClientCommandHook);

    InstallHooks();
}

void MW2Title::InitMenu()
{
    std::vector<OptionGroup> optionGroups;

    bool isFallDamageEnabled = Memory::Read<float>(0x82019C48) == 9999.0f;
    bool isUnlimitedAmmoEnabled = Memory::Read<uint32_t>(0x820E1724) == 0x7D284B78;
    bool areElevatorsEnabled = Memory::Read<uint16_t>(0x820D8360) == 0x4800;

    // Main section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ToggleOption, "God Mode", MW2::ToggleGodMode, false));
        options.emplace_back(MakeOption(ToggleOption, "Fall Damage", MW2::ToggleFallDamage, isFallDamageEnabled));
        options.emplace_back(MakeOption(ToggleOption, "Ammo", MW2::ToggleAmmo, isUnlimitedAmmoEnabled));
        options.emplace_back(MakeOption(ToggleOption, "Elevators", MW2::ToggleElevators, areElevatorsEnabled));
        options.emplace_back(MakeOption(ClickOption, "Spawn Care Package", MW2::SpawnCarePackage));
        options.emplace_back(MakeOption(ClickOption, "Spawn Blocker", MW2::SpawnBlocker));
        options.emplace_back(MakeOption(RangeOption<uint32_t>, "Knockback", MW2::Knockback, 1000, 0, 999999, 1000));
        options.emplace_back(MakeOption(RangeOption<uint32_t>, "Jump Height", MW2::ChangeJumpHeight, 39, 0, 999, 1));
        optionGroups.emplace_back(OptionGroup("Main", options));
    }

    // Teleport section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ToggleOption, "Save/Load Binds", MW2::ToggleSaveLoadBinds, &Context::BindsEnabled));
        options.emplace_back(MakeOption(ClickOption, "Save Position", MW2::SavePosition));
        options.emplace_back(MakeOption(ClickOption, "Load Position", MW2::LoadPosition));
        options.emplace_back(MakeOption(ToggleOption, "UFO", MW2::ToggleUfo, false));
        optionGroups.emplace_back(OptionGroup("Teleport", options));
    }

    // Bot section
    {
        std::vector<std::shared_ptr<Option>> options;
        options.emplace_back(MakeOption(ClickOption, "Spawn Bot", MW2::SpawnBot));
        options.emplace_back(MakeOption(ClickOption, "Teleport Bot to Me", MW2::TeleportBotToMe));
        options.emplace_back(MakeOption(ToggleOption, "Freeze Bot", MW2::ToggleBotMovement, true));
        optionGroups.emplace_back(OptionGroup("Bot", options));
    }

    // Set the save and load functions
    Context::SavePositionFn = MW2::SavePosition;
    Context::LoadPositionFn = MW2::LoadPosition;

    m_Menu.Init(optionGroups);
}

void MW2Title::Scr_NotifyHook(MW2::Game::gentity_s *entity, uint16_t stringValue, uint32_t paramCount)
{
    // Call the original Scr_Notify function
    s_DetourMap.at("Scr_Notify")->GetOriginal<decltype(&Scr_NotifyHook)>()(entity, stringValue, paramCount);

    // If the client is not host, no need to go further
    int clientNum = entity->state.number;
    if (!MW2::Game::IsHost(clientNum))
        return;

    // Get the string representing the event
    const char *eventName = MW2::Game::SL_ConvertToString(stringValue);

    // "begin" can happen multiple times a game in round-based gamemodes and we don't want
    // to recreate the menu every round so we make sure we are not already in a match
    if (!strcmp(eventName, "begin") && !s_CurrentInstance->InMatch())
    {
        Context::Reset();
        Context::ClientNum = clientNum;

        s_CurrentInstance->InMatch(true);
        s_CurrentInstance->InitMenu();

        // Disable the unlocalized error messages when printing something in the killfeed
        MW2::Game::SetClientDvar(clientNum, "loc_warnings", "0");
        MW2::Game::SetClientDvar(clientNum, "loc_warningsUI", "0");
    }
}

void MW2Title::SV_ExecuteClientCommandHook(int client, const char *s, int clientOK, int fromOldServer)
{
    // Call the original Scr_Notify SV_ExecuteClientCommand
    s_DetourMap.at("SV_ExecuteClientCommand")->GetOriginal<decltype(&SV_ExecuteClientCommandHook)>()(client, s, clientOK, fromOldServer);

    // If the client is not host, no need to go further
    int clientNum = (client - Memory::Read<int>(0x83623B98)) / 0x97F80;
    if (!MW2::Game::IsHost(clientNum))
        return;

    // Stop the menu when the game ends
    if (!strcmp(s, "disconnect"))
        s_CurrentInstance->InMatch(false);
}

void MW2Title::InitRenderer()
{
    using namespace Renderer;

    R_AddCmdDrawStretchPic = reinterpret_cast<R_ADDCMDDRAWSTRETCHPIC>(0x821384D8);
    R_AddCmdDrawText = reinterpret_cast<R_ADDCMDDRAWTEXT>(0x82350278);
    R_TextWidth = reinterpret_cast<R_TEXTWIDTH>(0x8234DD20);
    R_TextHeight = reinterpret_cast<R_TEXTHEIGHT>(0x8234DE10);
    R_RegisterFont = reinterpret_cast<R_REGISTERFONT>(0x8234DCB0);
    Material_RegisterHandle = reinterpret_cast<MATERIAL_REGISTERHANDLE>(0x8234E510);

    Title::InitRenderer();
}
