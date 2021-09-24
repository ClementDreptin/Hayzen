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

    virtual VOID Init() = 0;

    VOID Update();
protected:
    static Menu s_Menu;
    static Structure s_Structure;

    DWORD m_dwDrawTextFnAddr;
    DWORD m_dwDrawRectangleFnAddr;
    DWORD m_dwRegisterFontFnAddr;
    DWORD m_dwRegisterMaterialFnAddr;

    virtual VOID CreateStructure() = 0;
private:
    VOID SetDrawFunctionsPointers();
    VOID RegisterFontAndMaterial();
};
