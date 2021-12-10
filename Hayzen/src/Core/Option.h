#pragma once

#include "Elements\Text.h"


class Menu;

// Function called when an option is clicked.
typedef VOID (*Callback)(Menu* pMenu);


// Class describing a clickage option, inherits from Text.
class Option : public Text
{
public:
    // Default constructor.
    Option() {};

    // Constructor.
    Option(CONST std::string& strName, UINT uiIndex, Callback fnCallback = nullptr);

    // Call the callback function if it's defined, otherwise set the option as the current option in the menu.
    VOID OnClick(Menu* pMenu);

    // Push a child to the list of children.
    VOID AddChild(CONST std::shared_ptr<Option>& pChild) { pChild->m_pParent = this; m_Children.emplace_back(pChild); }

    BOOL HasParent() CONST { return m_pParent != nullptr; }
    Option* GetParent() CONST { return m_pParent; }

    CONST std::vector<std::shared_ptr<Option>>& GetChildren() CONST { return m_Children; }

    // Reset the members.
    VOID Cleanup();
private:
    UINT m_uiIndex;
    Callback m_fnCallback;

    Option* m_pParent;
    std::vector<std::shared_ptr<Option>> m_Children;
};


// Macro to create a shared pointer of an option.
#define MakeOption(...) std::make_shared<Option>(__VA_ARGS__)
