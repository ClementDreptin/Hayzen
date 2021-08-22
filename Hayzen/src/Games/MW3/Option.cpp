#include "pch.h"
#include "Games\MW3\Option.h"

namespace MW3
{
    Option::Option(INT clientNum, CONST std::string& name, INT index)
        : m_Index(index),
          TextElem(clientNum,
                name,
                HudElem::s_MenuX + HudElem::s_Padding,
                HudElem::s_MenuY + HudElem::s_Padding * 2 + HudElem::s_TitleHeight + HudElem::s_LineHeight * (index + 1),
                HudElem::s_ColorWhiteNoAlpha, 2.0f) {}
}