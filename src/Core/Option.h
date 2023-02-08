#pragma once

#include "Elements/Text.h"

class Menu;

typedef void (*Callback)(Menu *pMenu);

class Option : public Text
{
public:
    Option() {}

    Option(const std::string &name, uint32_t index, Callback callback = nullptr);

    void OnClick(Menu *pMenu);

    void AddChild(const std::shared_ptr<Option> &pChild);

    inline bool HasParent() const { return m_pParent != nullptr; }

    inline Option *GetParent() const { return m_pParent; }

    inline const std::vector<std::shared_ptr<Option>> &GetChildren() const { return m_Children; }

    void Cleanup();

private:
    uint32_t m_Index;
    Callback m_Callback;

    Option *m_pParent;
    std::vector<std::shared_ptr<Option>> m_Children;
};

// Macro to create a shared pointer of an option.
#define MakeOption(...) std::make_shared<Option>(__VA_ARGS__)
