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

    BOOL BindsEnabled() CONST { return m_bBindsEnabled; }
    VOID ToggleBinds() { m_bBindsEnabled = !m_bBindsEnabled; }

    CONST vec3& GetSavedPos() CONST { return m_SavedPos; }
    VOID SetSavedPos(CONST vec3& pos) { m_SavedPos = pos; }

    CONST vec3& GetSavedAngles() CONST { return m_SavedAngles; }
    VOID SetSavedAngles(CONST vec3& angles) { m_SavedAngles = angles; }

    VOID SetSavePositionFn(Callback fnSavePosition) { m_fnSavePosition = fnSavePosition; }
    VOID SetLoadPositionFn(Callback fnLoadPosition) { m_fnLoadPosition = fnLoadPosition; }

    LPVOID GetBot() CONST { return m_pBotEntity; }
    VOID SetBot(LPVOID pBotEntity) { m_pBotEntity = pBotEntity; }

    VOID SetFeedbackText(CONST std::string& strText);

    VOID SetCurrentOption(Option* pOption);
private:
    BOOL m_bInitialized;
    BOOL m_bOpen;

    INT m_iClientNum;

    Option* m_pCurrentOption;
    std::queue<Option*> m_ChangeSectionQueue;
    std::queue<std::string> m_ChangeFeedbackTextQueue;

    Rectangle m_Background;
    Text m_Title;
    Rectangle m_Scroller;
    Text m_Instructions;
    Text m_Feedback;
    INT m_iCurrentScrollerPos;

    vec3 m_SavedPos;
    vec3 m_SavedAngles;
    BOOL m_bBindsEnabled;

    Callback m_fnSavePosition;
    Callback m_fnLoadPosition;

    LPVOID m_pBotEntity;

    VOID MoveScroller();
};
