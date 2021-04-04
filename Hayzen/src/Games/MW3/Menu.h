#pragma once

#include "Games\MW3\HudElem.h"
#include "Games\MW3\Option.h"

namespace MW3
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
		vec3 m_SavedAngles;
		bool m_BindsEnabled;

		game_hudelem_s* m_Background;
		game_hudelem_s* m_Title;
		game_hudelem_s* m_Scroller;
		game_hudelem_s* m_Instructions;

		std::vector<Option> m_Options;
		static std::unordered_map<std::string, std::vector<std::string>> s_Structure;

		void Knockback();
		void ToggleDepatchBounces();
		void ToggleFallDamage();
		void ToggleAmmo();
		void ToggleOldSchool();
		void ToggleBlastMarks();
		void ToggleNoClip();
		void SavePosition();
		void LoadPosition();
		void ToggleSaveLoadBinds();
		void ToggleGodMode();
		void Verify(int clientNum);
		void SpawnCP();

		static void CreateStructure();
		void OnSelectPressed(const std::string& optionName);
		void OnBackPressed(const std::string& optionName);
		void Open();
		void Close();
		void GoToMenu(const std::string& menuName);
		void GetAllPlayers();
		static DWORD StaticKnockbackThread(LPVOID lpThreadParameter);
		void _Knockback();
		void MoveScroller(int position);
		void Cleanup();
	};
}