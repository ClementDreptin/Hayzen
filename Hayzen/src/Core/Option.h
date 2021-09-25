#pragma once

#include "Elements\Text.h"


// Forward declarationfor the Menu class
class Menu;

// Create the option callback type
typedef VOID (*Callback)(Menu* pMenu);


//--------------------------------------------------------------------------------------
// Name: class Option
// Desc: Class describing a clickage option, inherits from Text.
//--------------------------------------------------------------------------------------
class Option : public Text
{
public:
    Option() {};
    Option(CONST std::string& strName, UINT uiIndex, Callback fnCallback = nullptr);

    VOID OnClick(Menu* pMenu);

    VOID AddChild(CONST std::shared_ptr<Option>& pChild) { pChild->m_pParent = this; m_Children.emplace_back(pChild); }

    BOOL HasParent() CONST { return m_pParent != nullptr; }
    Option* GetParent() CONST { return m_pParent; }

    CONST std::vector<std::shared_ptr<Option>>& GetChildren() CONST { return m_Children; }
private:
    UINT m_uiIndex;
    Callback m_fnCallback;

    Option* m_pParent;
    std::vector<std::shared_ptr<Option>> m_Children;
};
