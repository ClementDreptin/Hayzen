#pragma once

#include "Games\Alpha\MW2\RectangleElem.h"
#include "Games\Alpha\MW2\TextElem.h"
#include "Games\Alpha\MW2\Option.h"

namespace Alpha
{
namespace MW2
{
    class Menu
    {
    public:
        friend class Option;

        Menu() {}
        Menu(INT clientNum);

        VOID OnEvent(CONST std::string& eventString);
        VOID Update();

        static VOID FreeBot();
    private:
        INT m_ClientNum;
        BOOL m_Open;
        INT m_CurrentScrollerPos;
        vec3 m_SavedPos;
        vec3 m_SavedAngles;
        BOOL m_BindsEnabled;

        RectangleElem m_Background;
        TextElem m_Title;
        RectangleElem m_Scroller;
        TextElem m_Instructions;

        static gentity_s* s_Bot;

        std::unordered_map<std::string, std::vector<Option>> m_Structure;

        VOID ToggleDepatchBounces();
        VOID ToggleFallDamage();
        VOID ToggleAmmo();
        VOID ToggleUFO();
        VOID SavePosition();
        VOID LoadPosition();
        VOID ToggleSaveLoadBinds();
        VOID ToggleGodMode();
        VOID SpawnCP();
        VOID SpawnBot();
        VOID TeleportBotToMe();
        VOID ToggleBotMovement();

        VOID CreateStructure();
        VOID OnSelectPressed(CONST std::string& optionName);
        VOID OnBackPressed(CONST std::string& optionName);
        VOID GoToMenu(CONST std::string& menuName);
        static DWORD StaticSpawnBotThread(LPVOID lpThreadParameter);
        VOID MoveScroller(INT position);
        VOID ResetScroller();
    };
}
}