#pragma once

#include "Core\Menu.h"


// Interface that game classes inherit from.
class Game
{
public:
    // Stop the menu and cleanup the root option when the game is destroyed.
    virtual ~Game();

    // Common initialization behavior to all games.
    virtual VOID Init() = 0;

    // Update and render the menu.
    VOID Update();
protected:
    static Menu s_Menu;
    static Option s_RootOption;

    DWORD m_dwDrawTextFnAddr;
    DWORD m_dwDrawRectangleFnAddr;
    DWORD m_dwRegisterFontFnAddr;
    DWORD m_dwRegisterMaterialFnAddr;

    // Create the menu structure (one implementation per game).
    virtual VOID CreateStructure() = 0;

    // Stub to hold the original code of SCR_DrawScreenField.
    static VOID SCR_DrawScreenFieldStub(CONST INT localClientNum, INT refreshedUI);

    // Render the menu.
    static VOID SCR_DrawScreenFieldHook(CONST INT localClientNum, INT refreshedUI);
private:
    // Make the global drawing function pointers point to the current game's drawing functions.
    VOID SetDrawFunctionsPointers();

    // Register a font and a material globally.
    VOID RegisterFontAndMaterial();
};
