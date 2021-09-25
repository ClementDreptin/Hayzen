#pragma once

#include "Core\Option.h"
#include "Elements\Rectangle.h"
#include "Elements\Text.h"


//--------------------------------------------------------------------------------------
// Name: class Menu
// Desc: Class to describe the general behavior of the menu.
//--------------------------------------------------------------------------------------
class Menu
{
public:
    Menu() {}

    VOID Init(INT iClientNum, Option* pOption);
    VOID Update();
    VOID Render();
    VOID Stop();

    BOOL IsInitialized() CONST { return m_bInitialized; }
    INT GetClientNum() CONST { return m_iClientNum; }

    VOID SetCurrentOption(Option* pOption);
private:
    BOOL m_bInitialized;
    BOOL m_bOpen;
    BOOL m_bRunning;

    INT m_iClientNum;

    Option* m_pCurrentOption;
    std::queue<Option*> m_ChangeSectionQueue;

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
