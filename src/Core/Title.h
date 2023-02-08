#pragma once

#include "Core/Menu.h"

class Title
{
public:
    virtual ~Title();

    virtual void Init() = 0;

protected:
    static Menu s_Menu;
    static Option s_RootOption;

    uintptr_t m_DrawTextFnAddr;
    uintptr_t m_DrawRectangleFnAddr;
    uintptr_t m_RegisterFontFnAddr;
    uintptr_t m_RegisterMaterialFnAddr;

    virtual void CreateStructure() = 0;

    static Detour *s_pSCR_DrawScreenFieldDetour;

    static void SCR_DrawScreenFieldHook(const int localClientNum, int refreshedUI);

private:
    void SetDrawFunctionsPointers();

    void RegisterFontAndMaterial();
};
