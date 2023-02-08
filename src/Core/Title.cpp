#include "pch.h"
#include "Core/Title.h"

#include "Elements/HudElem.h"

Menu Title::s_Menu;
Option Title::s_RootOption;
Detour *Title::s_pSCR_DrawScreenFieldDetour = nullptr;

Title::~Title()
{
    s_Menu.Stop();
    s_RootOption.Cleanup();

    delete s_pSCR_DrawScreenFieldDetour;
}

void Title::Init()
{
    // Make the HudElem draw function pointers point to the drawing functions of the current game
    SetDrawFunctionsPointers();

    // Register and font and a material globally with the drawing functions set above
    RegisterFontAndMaterial();
}

void Title::SCR_DrawScreenFieldHook(const int localClientNum, int refreshedUI)
{
    // Call the original SCR_DrawScreenField function
    s_pSCR_DrawScreenFieldDetour->GetOriginal<decltype(&SCR_DrawScreenFieldHook)>()(localClientNum, refreshedUI);

    // If the menu is not initialized, no need to go further
    if (!s_Menu.IsInitialized())
        return;

    // Update the menu
    s_Menu.Update();

    // Render the menu
    s_Menu.Render();
}

void Title::SetDrawFunctionsPointers()
{
    HudElem::R_RegisterFont = reinterpret_cast<R_REGISTERFONT>(m_RegisterFontFnAddr);
    HudElem::Material_RegisterHandle = reinterpret_cast<MATERIAL_REGISTERHANDLE>(m_RegisterMaterialFnAddr);

    HudElem::SetDrawTextFnPtr(reinterpret_cast<R_ADDCMDDRAWTEXT>(m_DrawTextFnAddr));
    HudElem::SetDrawRectangleFnPtr(reinterpret_cast<R_ADDCMDDRAWSTRETCHPIC>(m_DrawRectangleFnAddr));
}

void Title::RegisterFontAndMaterial()
{
    HudElem::SetFont(HudElem::R_RegisterFont("fonts/normalFont", 0));
    HudElem::SetMaterialHandle(HudElem::Material_RegisterHandle("white", 0));
}
