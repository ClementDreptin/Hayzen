#pragma once
#include "Games\MW2\HudElem.h"

namespace MW2
{
	class Option : public HudElem
	{
	public:
		Option() {};
		Option(int clientNum, const std::string& name, int index, const std::function<void()>& callback);

		inline std::string& GetName() { return m_Name; }
		inline game_hudelem_s* GetHudElem() const { return m_HudElem; }

		void OnClick();
	private:
		std::string m_Name;
		int m_Index;
		game_hudelem_s* m_HudElem;
		std::function<void()> m_Callback;
	};
}