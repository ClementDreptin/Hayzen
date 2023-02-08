#pragma once

#include "Core/Option.h"
#include "Elements/Rectangle.h"
#include "Elements/Text.h"

class Menu
{
public:
    Menu() {}

    void Init(int iClientNum, Option *pOption);

    void Update();

    void Render();

    void Stop();

    inline bool IsInitialized() const { return m_Initialized; }

    inline int GetClientNum() const { return m_ClientNum; }

    inline bool BindsEnabled() const { return m_BindsEnabled; }

    inline void ToggleBinds() { m_BindsEnabled = !m_BindsEnabled; }

    inline const vec3 &GetSavedPosition() const { return m_SavedPosition; }

    inline void SetSavedPosition(const vec3 &position) { m_SavedPosition = position; }

    inline const vec3 &GetSavedAngles() const { return m_SavedAngles; }

    inline void SetSavedAngles(const vec3 &angles) { m_SavedAngles = angles; }

    inline void SetSavePositionFn(Callback savePositionFn) { m_SavePositionFn = savePositionFn; }

    inline void SetLoadPositionFn(Callback loadPositionFn) { m_LoadPositionFn = loadPositionFn; }

    inline void *GetBot() const { return m_pBotEntity; }

    inline void SetBot(void *pBotEntity) { m_pBotEntity = pBotEntity; }

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

    void MoveScroller();
};
