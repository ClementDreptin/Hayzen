#pragma once
#include "Games\MW2\HudElem.h"

namespace MW2
{
	class Option : public HudElem
	{
	public:
		Option() {};
		Option(int clientNum, int teamNum, const std::string& name, int index);

		inline std::string& GetName() { return m_Name; }
		inline game_hudelem_s* GetHudElem() const { return m_HudElem; }
	private:
		std::string m_Name;
		game_hudelem_s* m_HudElem;
	};
}