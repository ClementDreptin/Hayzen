#include "pch.h"
#include "Core\Option.h"


//--------------------------------------------------------------------------------------
// Name: Option()
// Desc: Constructor.
//--------------------------------------------------------------------------------------
Option::Option(CONST std::string& strName, UINT uiIndex)
    : m_uiIndex(uiIndex),
    Text(strName,
        HudElem::s_MenuX + HudElem::s_Padding,
        HudElem::s_MenuY + HudElem::s_Padding * 2 + HudElem::s_TitleHeight + HudElem::s_LineHeight * (uiIndex + 1),
        HudElem::s_ColorWhite, 1.2f
    ) {}
