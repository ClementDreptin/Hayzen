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

		static const int m_X = 591;
		static const int m_Y = 240;
		static const int m_Width = 300;
		static const int m_Height = 470;
		static const int m_LineHeight = 30;

		void Open();
		void Close();
	};
}