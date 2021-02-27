#pragma once

#include "Games\MW2\Option.h"

namespace MW2
{
	class Menu
	{
	public:
		Menu() {}
		Menu(int clientNum, int teamNum);

		void Open();
		void Close();
	private:
		//std::vector<Option> m_Options;
		game_hudelem_s* m_HudElem;
	};
}