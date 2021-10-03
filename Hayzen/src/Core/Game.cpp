#include "pch.h"
#include "Core\Game.h"

#include "Elements\HudElem.h"


//--------------------------------------------------------------------------------------
// Static members definitions
//--------------------------------------------------------------------------------------
Menu Game::s_Menu;
Option Game::s_RootOption;


//--------------------------------------------------------------------------------------
// Name: ~Game()
// Desc: Stop the menu and cleanup the root option when the game is destroyed.
//--------------------------------------------------------------------------------------
Game::~Game()
{
    s_Menu.Stop();
    s_RootOption.Cleanup();
}


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
    if (!s_Menu.IsInitialized())
        return;

    s_Menu.Update();
}


//--------------------------------------------------------------------------------------
// Name: SCR_DrawScreenFieldHook()
// Desc: Render the menu.
//--------------------------------------------------------------------------------------
VOID Game::SCR_DrawScreenFieldHook(CONST INT localClientNum, INT refreshedUI)
{
    // Call the original SCR_DrawScreenField function
    SCR_DrawScreenFieldStub(localClientNum, refreshedUI);

    // Render the menu
    s_Menu.Render();
}


//--------------------------------------------------------------------------------------
// Name: SCR_DrawScreenFieldStub()
// Desc: Stub to hold the original code of SCR_DrawScreenField.
//--------------------------------------------------------------------------------------
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
