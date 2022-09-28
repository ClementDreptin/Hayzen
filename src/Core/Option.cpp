#include "pch.h"
#include "Core\Option.h"

#include "Core\Menu.h"

Option::Option(const std::string &name, uint32_t index, Callback callback)
    : m_Index(index),
      m_Callback(callback),
      Text(name, HudElem::s_MenuX + HudElem::s_Padding, HudElem::s_MenuY + HudElem::s_Padding * 2 + HudElem::s_TitleHeight + HudElem::s_LineHeight * (index + 1), HudElem::s_ColorWhite, 1.2f)
{
}

void Option::OnClick(Menu *pMenu)
{
    // If the callback is defined, it means the current option activates a feature
    // so we call the callback and return early
    if (m_Callback)
    {
        m_Callback(pMenu);
        return;
    }

    // If we reach this code, it means the current option redirects to another menu section.
    // It should have children but we still make sure it's the case just for safety.
    if (!m_Children.empty())
        pMenu->SetCurrentOption(this);
}

void Option::AddChild(const std::shared_ptr<Option> &pChild)
{
    pChild->m_pParent = this;
    m_Children.emplace_back(pChild);
}

void Option::Cleanup()
{
    m_Index = 0;
    m_Callback = nullptr;
    m_pParent = nullptr;
    m_Children.clear();
}
