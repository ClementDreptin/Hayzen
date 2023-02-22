#pragma once

class Title
{
public:
    Title();

    virtual ~Title();

protected:
    static Detour *s_pSCR_DrawScreenFieldDetour;

    static void SCR_DrawScreenFieldHook(const int localClientNum, int refreshedUI);
};
