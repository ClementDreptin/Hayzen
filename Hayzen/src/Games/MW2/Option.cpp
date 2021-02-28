#include "pch.h"
#include "Games\MW2\Option.h"

#include "Games\MW2\Functions.h"

namespace MW2
{
	Option::Option(int clientNum, int teamNum, const std::string& name, float x, float y)
		: m_Name(name)
	{
		m_HudElem = HudElem_Alloc(clientNum, teamNum);
		SetText(m_HudElem, name.c_str(), 2, x, y, 255, 255, 255, 0);
	}
}