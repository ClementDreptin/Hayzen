#include "pch.h"
#include "Games\SpecOps\MW3\Option.h"

#include "Games\SpecOps\MW3\Functions.h"

using namespace XexUtils;

namespace SpecOps
{
namespace MW3
{
	Option::Option(const std::string& name, int index, bool active)
		: m_Name(name), m_Index(index), m_Active(active)
	{
		TextElem(name,
			Menu::s_MenuX + Menu::s_MenuWidth / 2,
			Menu::s_MenuY + Menu::s_Padding * 2 + Menu::s_TitleHeight + Menu::s_LineHeight * index,
			active ? HudElem::s_ColorWhite : HudElem::s_ColorWhiteNoAlpha,
			1.2f);
	}

	void Option::Activate()
	{
		MakeAppear();
		m_Active = true;
	}

	void Option::Deactivate()
	{
		MakeDisappear();
		m_Active = false;
	}
}
}