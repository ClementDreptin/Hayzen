#pragma once

#include "Core/Option.h"
#include "Elements/Rectangle.h"
#include "Elements/Text.h"

// Class to describe the general behavior of the menu.
class Menu
{
public:
    // Default constructor.
    Menu() {}

    // Create the constant HUD elements.
    void Init(int iClientNum, Option *pOption);

    // Listen for controller inputs and update the menu accordingly.
    void Update();

    // Render every HUD element.
    void Render();

    // Stop the menu.
    void Stop();

    bool IsInitialized() const { return m_Initialized; }

    int GetClientNum() const { return m_ClientNum; }

    bool BindsEnabled() const { return m_BindsEnabled; }

    void ToggleBinds() { m_BindsEnabled = !m_BindsEnabled; }

    const vec3 &GetSavedPosition() const { return m_SavedPosition; }

    void SetSavedPosition(const vec3 &position) { m_SavedPosition = position; }

    const vec3 &GetSavedAngles() const { return m_SavedAngles; }

    void SetSavedAngles(const vec3 &angles) { m_SavedAngles = angles; }

    void SetSavePositionFn(Callback savePositionFn) { m_SavePositionFn = savePositionFn; }

    void SetLoadPositionFn(Callback loadPositionFn) { m_LoadPositionFn = loadPositionFn; }

    void *GetBot() const { return m_pBotEntity; }

    void SetBot(void *pBotEntity) { m_pBotEntity = pBotEntity; }

    // Change the current menu section.
    void SetCurrentOption(Option *pOption);

private:
    bool m_Initialized;
    bool m_Open;

    int m_ClientNum;

    Option *m_pCurrentOption;
    std::queue<Option *> m_ChangeSectionQueue;

    Rectangle m_Background;
    Text m_Title;
    Rectangle m_Scroller;
    Text m_Instructions;
    int m_CurrentScrollerPos;

    vec3 m_SavedPosition;
    vec3 m_SavedAngles;
    bool m_BindsEnabled;

    Callback m_SavePositionFn;
    Callback m_LoadPositionFn;

    void *m_pBotEntity;

    // Update the poition of the scroller according to the m_iCurrentScrollerPos index.
    void MoveScroller();
};
