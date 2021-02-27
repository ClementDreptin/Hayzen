#include "pch.h"
#include "Games\MW2\Option.h"

#include "Games\MW2\Functions.h"

namespace MW2
{
	Option::Option(int clientNum, int teamNum, const std::string& name)
		: m_Name(name)
	{
		m_HudElem = HudElem_Alloc(clientNum, teamNum);
	}

	Option::~Option()
	{
		HudElem_Free(m_HudElem);
	}
}