#include "pch.h"
#include "Games\SpecOps\MW3\Option.h"

#include "Games\SpecOps\MW3\Menu.h"

namespace SpecOps
{
namespace MW3
{
	Option::Option(const std::string& name, int index)
		: m_Index(index),
		  TextElem(name,
				Menu::s_MenuX + Menu::s_Padding,
				Menu::s_MenuY + Menu::s_Padding * 2 + Menu::s_TitleHeight + Menu::s_LineHeight * (index + 1),
				HudElem::s_ColorWhite, 1.2f) {}
}
}