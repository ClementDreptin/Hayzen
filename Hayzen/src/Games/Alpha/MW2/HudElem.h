#pragma once

#include "Games\Alpha\MW2\Structs.h"

namespace Alpha
{
namespace MW2
{
	extern const hudelem_color_t COLOR_WHITE;
	extern const hudelem_color_t COLOR_WHITE_NO_ALPHA;
	extern const hudelem_color_t COLOR_BLACK;
	extern const hudelem_color_t COLOR_BLACK_NO_ALPHA;

	class HudElem
	{
	public:
		HudElem() {}
		virtual ~HudElem() {}

	protected:
		static const int m_MenuX = 441;
		static const int m_MenuY = 5;
		static const int m_MenuWidth = 300;
		static const int m_MenuHeight = 470;
		static const int m_Padding = 10;
		static const int m_TitleHeight = 30;
		static const int m_LineHeight = 23;

		void SetShader(game_hudelem_s* elem, const char* materialName, int x, int y, int width, int height, hudelem_color_t color = COLOR_WHITE, float sort = 0.0f, align_t alignOrg = ALIGN_TOP_LEFT, align_t alignScreen = ALIGN_TOP_LEFT);

		void SetText(game_hudelem_s* elem, const char* text, float fontScale, int x, int y, hudelem_color_t color = COLOR_WHITE, int font = 4, float sort = 1.0f, align_t alignOrg = ALIGN_TOP_MIDDLE, align_t alignScreen = ALIGN_TOP_LEFT);

		void MakeAppear(game_hudelem_s* elem, byte alpha = 255);

		void MakeDisappear(game_hudelem_s* elem);
	};
}
}