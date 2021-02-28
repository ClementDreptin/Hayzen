#include "pch.h"
#include "Games\MW2\Option.h"

#include "Games\MW2\Functions.h"

namespace MW2
{
	Option::Option(int clientNum, int teamNum, const std::string& name, int index)
		: m_Name(name)
	{
		m_HudElem = HudElem_Alloc(clientNum, teamNum);
		SetText(m_HudElem, name.c_str(), 2, m_MenuX, m_Padding + m_TitleHeight + m_LineHeight * index, 255, 255, 255, 0);
	}
}