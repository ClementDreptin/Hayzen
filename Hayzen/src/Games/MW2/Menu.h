#pragma once
#include "Games\MW2\HudElem.h"

#include "Games\MW2\Option.h"

namespace MW2
{
	class Menu : public HudElem
	{
	public:
		Menu() {}
		Menu(int clientNum, int teamNum);

		void OnEvent(const std::string& eventString);
	private:
		bool m_Open;
		game_hudelem_s* m_Background;
		game_hudelem_s* m_Title;
		std::vector<Option> m_Options;

		void Open();
		void Close();
	};
}