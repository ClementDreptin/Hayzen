#pragma once

#include "Core\Option.h"
#include "Elements\Rectangle.h"
#include "Elements\Text.h"


// Create a short alias for the menu structure
typedef std::unordered_map<std::string, std::vector<Option>> Structure;


//--------------------------------------------------------------------------------------
// Name: class Menu
// Desc: Class to describe the general behavior of the menu.
//--------------------------------------------------------------------------------------
class Menu
{
public:
    Menu() {}

    VOID Init(INT iClientNum, Structure* pStructure);
    VOID Update();
    VOID Render();
    VOID Stop();

    BOOL IsInitialized() CONST { return m_bInitialized; }

    VOID GoToSection(CONST std::string& strSectionName);
private:
    BOOL m_bInitialized;
    BOOL m_bOpen;
    BOOL m_bRunning;

    INT m_iClientNum;
    Structure* m_pStructure;

    Rectangle m_Background;
    Text m_Title;
    Rectangle m_Scroller;
    Text m_Instructions;
    INT m_iCurrentScrollerPos;

    vec3 m_SavedPos;
    vec3 m_SavedAngles;
    BOOL m_bBindsEnabled;

    VOID MoveScroller();
};
