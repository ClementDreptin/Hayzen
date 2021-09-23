#pragma once

#include "Elements\Text.h"


// Create the option callback type
typedef VOID (*Callback)(LPVOID pData);


//--------------------------------------------------------------------------------------
// Name: class Option
// Desc: Class describing a clickage option, inherits from Text.
//--------------------------------------------------------------------------------------
class Option : public Text
{
public:
    Option() {};
    Option(CONST std::string& strName, UINT uiIndex);

    VOID OnClick(LPVOID pData) { m_fnCallback(pData); }
private:
    UINT m_uiIndex;
    Callback m_fnCallback;
};
