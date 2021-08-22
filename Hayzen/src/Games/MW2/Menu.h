#pragma once

#include "Games\MW2\RectangleElem.h"
#include "Games\MW2\TextElem.h"
#include "Games\MW2\Option.h"

namespace MW2
{
    class Menu
    {
    public:
        friend class Option;

        Menu() {}
        Menu(INT clientNum);

        VOID OnEvent(CONST std::string& eventString);

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

        VOID ToggleElevators();
        VOID Knockback();
        VOID ToggleDepatchBounces();
        VOID ToggleFallDamage();
        VOID ToggleAmmo();
        VOID ToggleUFO();
        VOID SavePosition();
        VOID LoadPosition();
        VOID ToggleSaveLoadBinds();
        VOID ToggleGodMode();
        VOID Verify(INT clientNum);
        VOID SpawnCP();
        VOID SpawnBot();
        VOID TeleportBotToMe();
        VOID ToggleBotMovement();

        VOID CreateStructure();
        VOID OnSelectPressed(CONST std::string& optionName);
        VOID OnBackPressed(CONST std::string& optionName);
        VOID Open();
        VOID Close();
        VOID GoToMenu(CONST std::string& menuName);
        VOID GetAllPlayers();
        static DWORD StaticKnockbackThread(LPVOID lpThreadParameter);
        static DWORD StaticSpawnBotThread(LPVOID lpThreadParameter);
        VOID _Knockback();
        VOID MoveScroller(INT position);
        VOID ResetScroller();
    };
}