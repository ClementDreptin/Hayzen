#include "pch.h"
#include "Core/Title.h"

Detour *Title::s_pSCR_DrawScreenFieldDetour = nullptr;

Title::Title()
{
}

Title::~Title()
{
    delete s_pSCR_DrawScreenFieldDetour;
}

void Title::SCR_DrawScreenFieldHook(const int localClientNum, int refreshedUI)
{
    // Call the original SCR_DrawScreenField function
    s_pSCR_DrawScreenFieldDetour->GetOriginal<decltype(&SCR_DrawScreenFieldHook)>()(localClientNum, refreshedUI);
}
