#pragma once

#include "Elements\Text.h"


// Create the option callback type
typedef VOID (*Callback)(LPVOID pData);

// Forward declaration for the menu
class Menu;


//--------------------------------------------------------------------------------------
// Name: class Option
// Desc: Class describing a clickage option, inherits from Text.
//--------------------------------------------------------------------------------------
class Option : public Text
{
public:
    Option() {};
    Option(CONST std::string& strName, UINT uiIndex, Callback fnCallback);

    VOID OnClick(Menu* pMenu) { m_fnCallback(pMenu); }
private:
    UINT m_uiIndex;
    Callback m_fnCallback;
};
