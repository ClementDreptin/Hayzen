#include "pch.h"
#include "Core\Game.h"

#include "Elements\HudElem.h"


Menu Game::s_Menu;
Option Game::s_RootOption;


Game::~Game()
{
    s_Menu.Stop();
    s_RootOption.Cleanup();
}

VOID Game::Init()
{
    // Make the HudElem draw function pointers point to the drawing functions of the current game
    SetDrawFunctionsPointers();

    // Register and font and a material globally with the drawing functions set above
    RegisterFontAndMaterial();
}

VOID Game::Update()
{
    // If the menu is not initialized, no need to go further
    if (!s_Menu.IsInitialized())
        return;

    s_Menu.Update();
}

VOID Game::SCR_DrawScreenFieldHook(CONST INT localClientNum, INT refreshedUI)
{
    // Call the original SCR_DrawScreenField function
    SCR_DrawScreenFieldStub(localClientNum, refreshedUI);

    // Render the menu
    s_Menu.Render();
}

VOID __declspec(naked) Game::SCR_DrawScreenFieldStub(CONST INT localClientNum, INT refreshedUI)
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

VOID Game::SetDrawFunctionsPointers()
{
    HudElem::R_RegisterFont = reinterpret_cast<R_REGISTERFONT>(m_dwRegisterFontFnAddr);
    HudElem::Material_RegisterHandle = reinterpret_cast<MATERIAL_REGISTERHANDLE>(m_dwRegisterMaterialFnAddr);

    HudElem::SetDrawTextFnPtr(reinterpret_cast<R_ADDCMDDRAWTEXT>(m_dwDrawTextFnAddr));
    HudElem::SetDrawRectangleFnPtr(reinterpret_cast<R_ADDCMDDRAWSTRETCHPIC>(m_dwDrawRectangleFnAddr));
}

VOID Game::RegisterFontAndMaterial()
{
    HudElem::SetFont(HudElem::R_RegisterFont("fonts/normalFont", 0));
    HudElem::SetMaterialHandle(HudElem::Material_RegisterHandle("white", 0));
}
