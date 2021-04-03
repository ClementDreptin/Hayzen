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

		static void FreeBot();
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

		static gentity_s* s_Bot;

		std::vector<Option> m_Options;
		static std::unordered_map<std::string, std::vector<std::string>> s_Structure;

		void ToggleElevators();
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
		void SpawnBot();
		void TeleportBotToMe();

		static void CreateStructure();
		void OnSelectPressed(const std::string& optionName);
		void OnBackPressed(const std::string& optionName);
		void Open();
		void Close();
		void GoToMenu(const std::string& menuName);
		void GetAllPlayers();
		static DWORD StaticKnockbackThread(LPVOID lpThreadParameter);
		static DWORD StaticSpawnBotThread(LPVOID lpThreadParameter);
		void _Knockback();
		void MoveScroller(int position);
		void Cleanup();
	};
}