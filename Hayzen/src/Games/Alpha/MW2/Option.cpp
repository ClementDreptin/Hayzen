#include "pch.h"
#include "Games\Alpha\MW2\Option.h"

namespace Alpha
{
namespace MW2
{
    Option::Option(const std::string& name, int index)
        : m_Index(index),
          TextElem(name,
                HudElem::s_MenuX + HudElem::s_Padding,
                HudElem::s_MenuY + HudElem::s_Padding * 2 + HudElem::s_TitleHeight + HudElem::s_LineHeight * (index + 1),
                HudElem::s_ColorWhite, 1.2f) {}
}
}