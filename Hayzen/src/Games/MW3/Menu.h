#pragma once

#include "Games\MW3\RectangleElem.h"
#include "Games\MW3\TextElem.h"
#include "Games\MW3\Option.h"

namespace MW3
{
    class Menu
    {
    public:
        friend class Option;

        Menu() {}
        Menu(int clientNum);

        void OnEvent(const std::string& eventString);
    private:
        int m_ClientNum;
        bool m_Open;
        int m_CurrentScrollerPos;
        vec3 m_SavedPos;
        vec3 m_SavedAngles;
        bool m_BindsEnabled;

        RectangleElem m_Background;
        TextElem m_Title;
        RectangleElem m_Scroller;
        TextElem m_Instructions;

        std::unordered_map<std::string, std::vector<Option>> m_Structure;

        void ToggleDepatchBounces();
        void ToggleFallDamage();
        void ToggleAmmo();
        void ToggleUFO();
        void SavePosition();
        void LoadPosition();
        void ToggleSaveLoadBinds();
        void ToggleGodMode();
        void Verify(int clientNum);
        void SpawnCP();

        void CreateStructure();
        void OnSelectPressed(const std::string& optionName);
        void OnBackPressed(const std::string& optionName);
        void Open();
        void Close();
        void GoToMenu(const std::string& menuName);
        void GetAllPlayers();
        void MoveScroller(int position);
        void ResetScroller();
    };
}