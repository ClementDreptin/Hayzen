#pragma once

#include "Games\MW3\Structs.h"

namespace MW3
{
	class HudElem
	{
	public:
		static const hudelem_color_t s_ColorWhite;
		static const hudelem_color_t s_ColorWhiteNoAlpha;
		static const hudelem_color_t s_ColorBlack;
		static const hudelem_color_t s_ColorBlackNoAlpha;
		static const float s_MenuX;
		static const float s_MenuY;
		static const int s_MenuWidth;
		static const int s_MenuHeight;
		static const float s_Padding;
		static const float s_TitleHeight;
		static const float s_LineHeight;

		HudElem() {}
		HudElem(int clientNum, float x, float y, const hudelem_color_t& color);
		virtual ~HudElem() {}

		float GetX() const { return m_InternalHudElem->elem.x; }
		float GetY() const { return m_InternalHudElem->elem.y; }
		const hudelem_color_t& GetColor() const { return m_InternalHudElem->elem.color; }

		void SetX(float x) { m_InternalHudElem->elem.x = x; }
		void SetY(float y) { m_InternalHudElem->elem.y = y; }
		void SetColor(const hudelem_color_t& color) { m_InternalHudElem->elem.color = color; }
		void SetColor(byte r, byte g, byte b, byte a);
		void SetAlpha(byte alpha) { m_InternalHudElem->elem.color.a = alpha; }
	protected:
		game_hudelem_s* m_InternalHudElem;
	};
}