#pragma once
#include "Games\Alpha\MW2\HudElem.h"

#include "Games\Alpha\MW2\Option.h"

namespace Alpha
{
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
		vec3 m_SavedPos;
		bool m_BindsEnabled;

		game_hudelem_s* m_Background;
		game_hudelem_s* m_Title;
		game_hudelem_s* m_Scroller;

		std::vector<Option> m_Options;
		static std::unordered_map<std::string, std::vector<std::string>> s_Structure;

		void Knockback();
		void ToggleDepatchBounces();
		void ToggleFallDamage();
		void ToggleAmmo();
		void ToggleOldSchool();
		void ToggleBlastMarks();
		void ToggleUFO();
		void SavePosition();
		void LoadPosition();
		void ToggleSaveLoadBinds();
		void ToggleGodMode();
		void Verify(int clientNum);
		void SpawnCP();

		static void CreateStructure();
		void OnAPressed(const std::string& optionName);
		void OnBPressed(const std::string& optionName);
		void Open();
		void Close();
		void GoToMenu(const std::string& menuName);
		void GetAllPlayers();
		static DWORD StaticKnockbackThread(LPVOID lpThreadParameter);  
		void _Knockback();
		void ToDo(); // Remove when all features are implemented
		void MoveScroller(int position);
		void Cleanup();
	};
}
}