#pragma once

#include "Games\SpecOps\MW3\RectangleElem.h"
#include "Games\SpecOps\MW3\TextElem.h"
#include "Games\SpecOps\MW3\Option.h"

namespace SpecOps
{
namespace MW3
{
	class Menu
	{
	public:
		friend class Option;

		Menu() {}
		Menu(int clientNum);

		void OnEvent(const std::string& eventString);
		void Update();
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

		std::unordered_map<std::string, std::vector<Option>> m_Structure;

		static const float s_MenuX;
		static const float s_MenuY;
		static const float s_MenuWidth;
		static const float s_MenuHeight;
		static const float s_Padding;
		static const float s_TitleHeight;
		static const float s_LineHeight;

		void ToggleGodMode();
		void ToggleAmmo();
		void ToggleUFO();
		void SavePosition();
		void LoadPosition();
		void ToggleSaveLoadBinds();

		void CreateStructure();
		void OnSelectPressed(const std::string& optionName);
		void OnBackPressed(const std::string& optionName);
		void Open();
		void Close();
		void GoToMenu(const std::string& menuName);
		void MoveScroller(int position);
		void ResetCursor();
	};
}
}