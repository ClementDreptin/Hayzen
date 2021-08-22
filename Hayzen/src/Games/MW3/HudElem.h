#pragma once

#include "Games\MW3\Structs.h"

namespace MW3
{
    class HudElem
    {
    public:
        static CONST hudelem_color_t s_ColorWhite;
        static CONST hudelem_color_t s_ColorWhiteNoAlpha;
        static CONST hudelem_color_t s_ColorBlack;
        static CONST hudelem_color_t s_ColorBlackNoAlpha;
        static CONST FLOAT s_MenuX;
        static CONST FLOAT s_MenuY;
        static CONST INT s_MenuWidth;
        static CONST INT s_MenuHeight;
        static CONST FLOAT s_Padding;
        static CONST FLOAT s_TitleHeight;
        static CONST FLOAT s_LineHeight;

        HudElem() {}
        HudElem(INT clientNum, FLOAT x, FLOAT y, CONST hudelem_color_t& color);
        virtual ~HudElem() {}

        FLOAT GetX() CONST { return m_InternalHudElem->elem.x; }
        FLOAT GetY() CONST { return m_InternalHudElem->elem.y; }
        CONST hudelem_color_t& GetColor() CONST { return m_InternalHudElem->elem.color; }

        VOID SetX(FLOAT x) { m_InternalHudElem->elem.x = x; }
        VOID SetY(FLOAT y) { m_InternalHudElem->elem.y = y; }
        VOID SetColor(CONST hudelem_color_t& color) { m_InternalHudElem->elem.color = color; }
        VOID SetColor(BYTE r, BYTE g, BYTE b, BYTE a);
        VOID SetAlpha(BYTE alpha) { m_InternalHudElem->elem.color.a = alpha; }
    protected:
        game_hudelem_s* m_InternalHudElem;
    };
}