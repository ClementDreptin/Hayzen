#include "pch.h"
#include "Games\MW3\Option.h"

#include "Games\MW3\Functions.h"

namespace MW3
{
	Option::Option(int clientNum, const std::string& name, int index, bool active)
		: m_Name(name), m_Index(index), m_Active(active)
	{
		m_HudElem = HudElem_Alloc(clientNum, 0);
		SetText(m_HudElem, name.c_str(), 2, m_MenuX + m_MenuWidth / 2, m_MenuY + m_Padding * 2 + m_TitleHeight + m_LineHeight * index, active ? COLOR_WHITE : COLOR_WHITE_NO_ALPHA);
	}

	void Option::FreeHudElem()
	{
		HudElem_Free(m_HudElem);
	}

	void Option::Activate()
	{
		MakeAppear(m_HudElem);
		m_Active = true;
	}

	void Option::Deactivate()
	{
		MakeDisappear(m_HudElem);
		m_Active = false;
	}
}