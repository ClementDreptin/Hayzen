#pragma once

#include "Games\MW3\HudElem.h"

namespace MW3
{
	class Option : public HudElem
	{
	public:
		Option() {};
		Option(int clientNum, const std::string& name, int index, bool active = false);

		inline std::string GetName() { return m_Name; }
		inline game_hudelem_s* GetHudElem() const { return m_HudElem; }

		void FreeHudElem();
		void Activate();
		void Deactivate();
	private:
		std::string m_Name;
		int m_Index;
		game_hudelem_s* m_HudElem;
		bool m_Active;
	};
}