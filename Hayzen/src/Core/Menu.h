#pragma once

#include "Core\Option.h"
#include "Elements\Rectangle.h"
#include "Elements\Text.h"


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

    bool IsInitialized() const { return m_bInitialized; }
    int GetClientNum() const { return m_iClientNum; }

    bool BindsEnabled() const { return m_bBindsEnabled; }
    void ToggleBinds() { m_bBindsEnabled = !m_bBindsEnabled; }

    const vec3 &GetSavedPos() const { return m_SavedPos; }
    void SetSavedPos(const vec3 &pos) { m_SavedPos = pos; }

    const vec3 &GetSavedAngles() const { return m_SavedAngles; }
    void SetSavedAngles(const vec3 &angles) { m_SavedAngles = angles; }

    bool HasSavedPosition() const { return !m_SavedPos.isNull() && !m_SavedAngles.isNull(); }

    bool IsTeleportRequested() const { return m_bTeleportRequested; }
    void RequestTeleport() { m_bTeleportRequested = true; }
    void NotifyTeleportCompletion() { m_bTeleportRequested = false; }

    void SetSavePositionFn(Callback fnSavePosition) { m_fnSavePosition = fnSavePosition; }
    void SetLoadPositionFn(Callback fnLoadPosition) { m_fnLoadPosition = fnLoadPosition; }

    void *GetBot() const { return m_pBotEntity; }
    void SetBot(void *pBotEntity) { m_pBotEntity = pBotEntity; }

    // Change the current menu section.
    void SetCurrentOption(Option *pOption);
private:
    bool m_bInitialized;
    bool m_bOpen;

    int m_iClientNum;

    Option *m_pCurrentOption;
    std::queue<Option *> m_ChangeSectionQueue;

    Rectangle m_Background;
    Text m_Title;
    Rectangle m_Scroller;
    Text m_Instructions;
    int m_iCurrentScrollerPos;

    vec3 m_SavedPos;
    vec3 m_SavedAngles;
    bool m_bBindsEnabled;
    bool m_bTeleportRequested;

    Callback m_fnSavePosition;
    Callback m_fnLoadPosition;

    void *m_pBotEntity;

    // Update the poition of the scroller according to the m_iCurrentScrollerPos index.
    void MoveScroller();
};
