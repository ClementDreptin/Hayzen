#include "pch.h"
#include "Games\MW2\HudElem.h"

#include "Games\MW2\Functions.h"

namespace MW2
{
    CONST hudelem_color_t HudElem::s_ColorWhite = { 255, 255, 255, 255 };
    CONST hudelem_color_t HudElem::s_ColorWhiteNoAlpha = { 255, 255, 255, 0 };
    CONST hudelem_color_t HudElem::s_ColorBlack = { 0, 0, 0, 255 };
    CONST hudelem_color_t HudElem::s_ColorBlackNoAlpha = { 0, 0, 0, 0 };
    CONST FLOAT HudElem::s_MenuX = 441.0f;
    CONST FLOAT HudElem::s_MenuY = 5.0f;
    CONST INT HudElem::s_MenuWidth = 300;
    CONST INT HudElem::s_MenuHeight = 470;
    CONST FLOAT HudElem::s_Padding = 10.0f;
    CONST FLOAT HudElem::s_TitleHeight = 30.0f;
    CONST FLOAT HudElem::s_LineHeight = 23.0f;

    HudElem::HudElem(INT clientNum, FLOAT x, FLOAT y, CONST hudelem_color_t& color)
    {
        m_InternalHudElem = HudElem_Alloc(clientNum, 0);
        SetX(x);
        SetY(y);
        SetColor(color);
    }

    VOID HudElem::SetColor(BYTE r, BYTE g, BYTE b, BYTE a)
    {
        m_InternalHudElem->elem.color.r = r;
        m_InternalHudElem->elem.color.g = g;
        m_InternalHudElem->elem.color.b = b;
        m_InternalHudElem->elem.color.a = a;
    }
}