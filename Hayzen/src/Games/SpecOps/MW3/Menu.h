#pragma once

#include "Games\SpecOps\MW3\RectangleElem.h"
#include "Games\SpecOps\MW3\TextElem.h"
#include "Games\SpecOps\MW3\Option.h"

namespace SpecOps
{
namespace MW3
{
    class Menu
    {
    public:
        friend class Option;

        Menu() {}
        Menu(INT clientNum);

        VOID OnEvent(CONST std::string& eventString);
        VOID Update();
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

        std::unordered_map<std::string, std::vector<Option>> m_Structure;

        VOID ToggleGodMode();
        VOID ToggleAmmo();
        VOID ToggleUFO();
        VOID SavePosition();
        VOID LoadPosition();
        VOID ToggleSaveLoadBinds();

        VOID CreateStructure();
        VOID OnSelectPressed(CONST std::string& optionName);
        VOID OnBackPressed(CONST std::string& optionName);
        VOID GoToMenu(CONST std::string& menuName);
        VOID MoveScroller(INT position);
        VOID ResetScroller();
    };
}
}