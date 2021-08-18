#pragma once

#include "Games\SpecOps\MW2\RectangleElem.h"
#include "Games\SpecOps\MW2\TextElem.h"
#include "Games\SpecOps\MW2\Option.h"

namespace SpecOps
{
namespace MW2
{
    class Menu
    {
    public:
        friend class Option;

        Menu() {}
        Menu(int clientNum);

        void OnEvent(const std::string& eventString);
        void Update();
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

        void ToggleGodMode();
        void ToggleAmmo();
        void ToggleUFO();
        void SavePosition();
        void LoadPosition();
        void ToggleSaveLoadBinds();

        void CreateStructure();
        void OnSelectPressed(const std::string& optionName);
        void OnBackPressed(const std::string& optionName);
        void GoToMenu(const std::string& menuName);
        void MoveScroller(int position);
        void ResetScroller();
    };
}
}