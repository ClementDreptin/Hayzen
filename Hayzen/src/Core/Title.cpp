#include "pch.h"
#include "Core\Title.h"

#include "Elements\HudElem.h"


Menu Title::s_Menu;
Option Title::s_RootOption;


Title::~Title()
{
    s_Menu.Stop();
    s_RootOption.Cleanup();
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
    SCR_DrawScreenFieldStub(localClientNum, refreshedUI);

    // If the menu is not initialized, no need to go further
    if (!s_Menu.IsInitialized())
        return;

    // Update the menu
    s_Menu.Update();

    // Render the menu
    s_Menu.Render();
}

void __declspec(naked) Title::SCR_DrawScreenFieldStub(const int localClientNum, int refreshedUI)
{
    __asm
    {
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        li r3, 0
    }
}

void Title::SetDrawFunctionsPointers()
{
    HudElem::R_RegisterFont = reinterpret_cast<R_REGISTERFONT>(m_dwRegisterFontFnAddr);
    HudElem::Material_RegisterHandle = reinterpret_cast<MATERIAL_REGISTERHANDLE>(m_dwRegisterMaterialFnAddr);

    HudElem::SetDrawTextFnPtr(reinterpret_cast<R_ADDCMDDRAWTEXT>(m_dwDrawTextFnAddr));
    HudElem::SetDrawRectangleFnPtr(reinterpret_cast<R_ADDCMDDRAWSTRETCHPIC>(m_dwDrawRectangleFnAddr));
}

void Title::RegisterFontAndMaterial()
{
    HudElem::SetFont(HudElem::R_RegisterFont("fonts/normalFont", 0));
    HudElem::SetMaterialHandle(HudElem::Material_RegisterHandle("white", 0));
}