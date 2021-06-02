#pragma once

#include "Games\Alpha\MW2\RectangleElem.h"
#include "Games\Alpha\MW2\TextElem.h"
#include "Games\Alpha\MW2\Option.h"

namespace Alpha
{
namespace MW2
{
	class Menu
	{
	public:
		friend class Option;

		Menu() {}
		Menu(int clientNum);

		void OnEvent(const std::string& eventString);
		void Update();

		static void FreeBot();
	private:
		int m_ClientNum;
		bool m_Open;
		int m_CurrentScrollerPos;
		vec3 m_SavedPos;
		vec3 m_SavedAngles;
		bool m_BindsEnabled;

		RectangleElem m_Background;
		TextElem m_Title;
		RectangleElem m_Scroller;
		TextElem m_Instructions;

		static gentity_s* s_Bot;

		std::unordered_map<std::string, std::vector<Option>> m_Structure;

		void ToggleDepatchBounces();
		void ToggleFallDamage();
		void ToggleAmmo();
		void ToggleUFO();
		void SavePosition();
		void LoadPosition();
		void ToggleSaveLoadBinds();
		void ToggleGodMode();
		void SpawnCP();
		void SpawnBot();
		void TeleportBotToMe();
		void ToggleBotMovement();

		void CreateStructure();
		void OnSelectPressed(const std::string& optionName);
		void OnBackPressed(const std::string& optionName);
		void GoToMenu(const std::string& menuName);
		static DWORD StaticSpawnBotThread(LPVOID lpThreadParameter);
		void MoveScroller(int position);
		void ResetScroller();
	};
}
}