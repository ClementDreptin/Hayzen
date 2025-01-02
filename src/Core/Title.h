#pragma once

#include "Core/Menu.h"

class Title
{
public:
    Title();

    virtual ~Title();

    inline bool InMatch() const { return m_InMatch; }

    inline void InMatch(bool inMatch) { m_InMatch = inMatch; }

    inline static std::unordered_map<std::string, Detour> &GetDetourMap() { return s_DetourMap; }

    virtual void InitMenu() = 0;

protected:
    Menu m_Menu;
    bool m_MenuOpen;

    static Title *s_CurrentInstance;

    static std::unordered_map<std::string, Detour> s_DetourMap;

    static void SCR_DrawScreenFieldHook(const int localClientNum, int refreshedUI);

    void InstallHooks();

    void RemoveHooks();

    virtual void InitRenderer() = 0;

private:
    bool m_InMatch;

    void Update();

    void Render();

    void RenderControlsTexts();
};
