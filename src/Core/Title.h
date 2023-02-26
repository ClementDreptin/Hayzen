#pragma once

#include "Core/Menu.h"
#include "UI/Text.h"

class Title
{
public:
    Title();

    virtual ~Title();

    inline bool InMatch() const { return m_InMatch; }

    inline void InMatch(bool inMatch) { m_InMatch = inMatch; }

    virtual void InitMenu() = 0;

protected:
    Menu m_Menu;
    bool m_MenuOpen;

    static Title *s_CurrentInstance;

    static Detour *s_pSCR_DrawScreenFieldDetour;

    static void SCR_DrawScreenFieldHook(const int localClientNum, int refreshedUI);

    virtual void InitRenderer() = 0;

private:
    bool m_InMatch;

    Text m_ControlsTexts[3];

    void Update();

    void Render();

    void RenderControlsTexts();
};
