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
		static std::unordered_map<std::string, std::vector<std::string>> s_Structure;

		void OptionClicked(const std::string& optionName);

		void Open();
		void Close();
		static void CreateStructure();
		void GoToMenu(const std::string& menuName);
		void OnAPressed(const std::string& optionName);
		void OnBPressed(const std::string& optionName);
		void MoveScroller(int position);
		void Cleanup();
	};
}