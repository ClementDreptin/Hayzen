#pragma once

#include "UI/Text.h"

class Title
{
public:
    Title();

    virtual ~Title();

    inline bool InMatch() const { return m_InMatch; }

    inline void InMatch(bool inMatch) { m_InMatch = inMatch; }

protected:
    static Title *s_CurrentInstance;

    static Detour *s_pSCR_DrawScreenFieldDetour;

    static void SCR_DrawScreenFieldHook(const int localClientNum, int refreshedUI);

    virtual void InitRenderer() = 0;

private:
    bool m_InMatch;
    bool m_MenuOpen;

    Text m_ControlsText;

    void Update();

    void Render();

    void RenderControlsText();
};
