#pragma once

#include "Elements/Text.h"

class Menu;

// Function called when an option is clicked.
typedef void (*Callback)(Menu *pMenu);

// Class describing a clickage option, inherits from Text.
class Option : public Text
{
public:
    // Default constructor.
    Option() {}

    // Constructor.
    Option(const std::string &name, uint32_t index, Callback callback = nullptr);

    // Call the callback function if it's defined, otherwise set the option as the current option in the menu.
    void OnClick(Menu *pMenu);

    // Push a child to the list of children.
    void AddChild(const std::shared_ptr<Option> &pChild);

    bool HasParent() const { return m_pParent != nullptr; }

    Option *GetParent() const { return m_pParent; }

    const std::vector<std::shared_ptr<Option>> &GetChildren() const { return m_Children; }

    // Reset the members.
    void Cleanup();

private:
    uint32_t m_Index;
    Callback m_Callback;

    Option *m_pParent;
    std::vector<std::shared_ptr<Option>> m_Children;
};

// Macro to create a shared pointer of an option.
#define MakeOption(...) std::make_shared<Option>(__VA_ARGS__)
