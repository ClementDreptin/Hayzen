#pragma once

#include "Elements\Text.h"


// Forward declarations
class Menu;
class Option;

// Create the option callback type
typedef VOID (*Callback)(Menu* pMenu, Option* pContext);


//--------------------------------------------------------------------------------------
// Name: class Option
// Desc: Class describing a clickage option, inherits from Text.
//--------------------------------------------------------------------------------------
class Option : public Text
{
public:
    Option() {};
    Option(CONST std::string& strName, UINT uiIndex, Callback fnCallback);

    VOID OnClick(Menu* pMenu) { m_fnCallback(pMenu, this); }
private:
    UINT m_uiIndex;
    Callback m_fnCallback;
};
