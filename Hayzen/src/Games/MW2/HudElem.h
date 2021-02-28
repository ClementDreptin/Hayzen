#pragma once

#include "Games\MW2\Structs.h"

namespace MW2
{
	class HudElem
	{
	public:
		HudElem() {}
		virtual ~HudElem() {}

	protected:
		static const int m_MenuX = 591;
		static const int m_MenuY = 240;
		static const int m_MenuWidth = 300;
		static const int m_MenuHeight = 470;
		static const int m_Padding = 7;
		static const int m_TitleHeight = 40;
		static const int m_LineHeight = 30;

		void SetShader(game_hudelem_s* elem, const char* materialName, int x, int y, int width, int height, byte r = 255, byte g = 255, byte b = 255, byte a = 255, float sort = 0.0f, int alignOrg = 5, int alignScreen = 0);

		void SetText(game_hudelem_s* elem, const char* text, float fontScale, int x, int y, byte r = 255, byte g = 255, byte b = 255, byte a = 255, int font = 4, float sort = 1.0f, int alignOrg = 5, int alignScreen = 1);

		void MakeAppear(game_hudelem_s* elem, byte alpha = 255);

		void MakeDisappear(game_hudelem_s* elem);
	};
}