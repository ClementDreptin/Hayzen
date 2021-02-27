#pragma once

#include "Games\MW2\Structs.h"

namespace MW2
{
	class Option
	{
	public:
		Option() {};
		Option(int clientNum, int teamNum, const std::string& name);
		~Option();
	private:
		std::string m_Name;
		game_hudelem_s* m_HudElem;
	};
}