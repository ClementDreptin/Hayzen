#include "pch.h"
#include "Core\Option.h"

#include "Core\Menu.h"

Option::Option(const std::string &strName, uint32_t uiIndex, Callback fnCallback)
    : m_uiIndex(uiIndex),
      m_fnCallback(fnCallback),
      Text(strName, HudElem::s_MenuX + HudElem::s_Padding, HudElem::s_MenuY + HudElem::s_Padding * 2 + HudElem::s_TitleHeight + HudElem::s_LineHeight * (uiIndex + 1), HudElem::s_ColorWhite, 1.2f)
{
}

void Option::OnClick(Menu *pMenu)
{
    // If the callback is defined, it means the current option activates a feature
    // so we call the callback and return early
    if (m_fnCallback)
    {
        m_fnCallback(pMenu);
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
    m_uiIndex = 0;
    m_fnCallback = nullptr;
    m_pParent = nullptr;
    m_Children.clear();
}
