#include "pch.h"
#include "Games\Alpha\MW2\Option.h"

#include "Games\Alpha\MW2\Menu.h"

namespace Alpha
{
namespace MW2
{
	Option::Option(const std::string& name, int index)
		: m_Index(index),
		  TextElem(name,
				Menu::s_MenuX + Menu::s_Padding,
				Menu::s_MenuY + Menu::s_Padding * 2 + Menu::s_TitleHeight + Menu::s_LineHeight * (index + 1),
				HudElem::s_ColorWhite, 1.2f) {}
}
}