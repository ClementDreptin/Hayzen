#pragma once
#include "Games\MW2\HudElem.h"

#include "Games\MW2\Option.h"

namespace MW2
{
	class Menu : public HudElem
	{
	public:
		Menu() {}
		Menu(int clientNum);

		void OnEvent(const std::string& eventString);
	private:
		int m_ClientNum;
		bool m_Open;
		int m_CurrentScrollerPos;

		game_hudelem_s* m_Background;
		game_hudelem_s* m_Title;
		game_hudelem_s* m_Scroller;

		std::vector<Option> m_Options;

		void Open();
		void Close();
		void MoveScroller(int position);

		void GoToRoot();
		void GoToMain();
		void GoToTeleport();
		void GoToInfect();

		void OptionClicked();

		void OnAPressed(const std::string& optionName);
		void OnBPressed(const std::string& optionName);

		void Cleanup();
	};
}