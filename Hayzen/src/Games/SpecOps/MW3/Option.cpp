#include "pch.h"
#include "Games\SpecOps\MW3\Option.h"

namespace SpecOps
{
namespace MW3
{
    Option::Option(CONST std::string& name, INT index)
        : m_Index(index),
          TextElem(name,
                HudElem::s_MenuX + HudElem::s_Padding,
                HudElem::s_MenuY + HudElem::s_Padding * 2 + HudElem::s_TitleHeight + HudElem::s_LineHeight * (index + 1),
                HudElem::s_ColorWhite, 1.2f) {}
}
}