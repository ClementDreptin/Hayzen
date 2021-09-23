#include "pch.h"
#include "Core\Game.h"

#include "Elements\HudElem.h"


//--------------------------------------------------------------------------------------
// Name: Init()
// Desc: Common initialization behavior to all games.
//--------------------------------------------------------------------------------------
VOID Game::Init()
{
    // Make the HudElem draw function pointers point to the drawing functions of the current game
    SetDrawFunctionsPointers();

    // Register and font and a material globally with the drawing functions set above
    RegisterFontAndMaterial();
}


//--------------------------------------------------------------------------------------
// Name: Update()
// Desc: Update and render the menu.
//--------------------------------------------------------------------------------------
VOID Game::Update()
{
    // If the menu is not initialized, no need to go further
    if (!m_Menu.IsInitialized())
        return;

    m_Menu.Update();

    m_Menu.Render();
}


//--------------------------------------------------------------------------------------
// Name: SetDrawFunctionsPointers()
// Desc: Make the global drawing function pointers point to the current game's drawing
//       functions.
//--------------------------------------------------------------------------------------
VOID Game::SetDrawFunctionsPointers()
{
    HudElem::R_RegisterFont = (R_REGISTERFONT)m_dwRegisterFontFnAddr;
    HudElem::Material_RegisterHandle = (MATERIAL_REGISTERHANDLE)m_dwRegisterMaterialFnAddr;

    HudElem::SetDrawTextFnPtr((R_ADDCMDDRAWTEXT)m_dwDrawTextFnAddr);
    HudElem::SetDrawRectangleFnPtr((R_ADDCMDDRAWSTRETCHPIC)m_dwDrawRectangleFnAddr);
}


//--------------------------------------------------------------------------------------
// Name: RegisterFontAndMaterial()
// Desc: Register a font and a material globally.
//--------------------------------------------------------------------------------------
VOID Game::RegisterFontAndMaterial()
{
    HudElem::SetFont(HudElem::R_RegisterFont("fonts/normalFont", 0));
    HudElem::SetMaterialHandle(HudElem::Material_RegisterHandle("white", 0));
}
