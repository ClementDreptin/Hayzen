#pragma once

#include "Core\Menu.h"


//--------------------------------------------------------------------------------------
// Name: class Game
// Desc: Interface that game classes inherit from.
//--------------------------------------------------------------------------------------
class Game
{
public:
    virtual ~Game() {}

    virtual VOID Init();

    VOID Update();
protected:
    DWORD m_dwDrawTextFnAddr;
    DWORD m_dwDrawRectangleFnAddr;
    DWORD m_dwRegisterFontFnAddr;
    DWORD m_dwRegisterMaterialFnAddr;

    Menu m_Menu;

    VOID SetDrawFunctionsPointers();
    VOID RegisterFontAndMaterial();
};
