#pragma once
#include "Games\MW2\HudElem.h"

#include "Games\MW2\Option.h"

namespace MW2
{
	class Menu : public HudElem
	{
	public:
		Menu() {}
		Menu(int clientNum, int teamNum);

		void Open();
		void Close();

		inline bool IsOpen() const { return m_Open; }
	private:
		game_hudelem_s* m_Background;
		bool m_Open;
	};
}