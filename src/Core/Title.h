#pragma once

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

private:
    virtual void Update() = 0;

    bool m_InMatch;
};
