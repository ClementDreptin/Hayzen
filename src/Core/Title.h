#pragma once

#include "Core\Menu.h"

// Interface that game classes inherit from.
class Title
{
public:
    // Stop the menu and cleanup the root option when the game is destroyed.
    virtual ~Title();

    // Common initialization behavior to all games.
    virtual void Init() = 0;

protected:
    static Menu s_Menu;
    static Option s_RootOption;

    DWORD m_dwDrawTextFnAddr;
    DWORD m_dwDrawRectangleFnAddr;
    DWORD m_dwRegisterFontFnAddr;
    DWORD m_dwRegisterMaterialFnAddr;

    // Create the menu structure (one implementation per game).
    virtual void CreateStructure() = 0;

    // Pointer to the detour object for SCR_DrawScreenField.
    static Detour *s_pSCR_DrawScreenFieldDetour;

    // Hook of a function that gets called every frame to execute the main loop on the game's thread.
    static void SCR_DrawScreenFieldHook(const int localClientNum, int refreshedUI);

private:
    // Make the global drawing function pointers point to the current game's drawing functions.
    void SetDrawFunctionsPointers();

    // Register a font and a material globally.
    void RegisterFontAndMaterial();
};
