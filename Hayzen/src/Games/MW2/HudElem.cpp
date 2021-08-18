#include "pch.h"
#include "Games\MW2\HudElem.h"

#include "Games\MW2\Functions.h"

namespace MW2
{
    const hudelem_color_t HudElem::s_ColorWhite = { 255, 255, 255, 255 };
    const hudelem_color_t HudElem::s_ColorWhiteNoAlpha = { 255, 255, 255, 0 };
    const hudelem_color_t HudElem::s_ColorBlack = { 0, 0, 0, 255 };
    const hudelem_color_t HudElem::s_ColorBlackNoAlpha = { 0, 0, 0, 0 };
    const float HudElem::s_MenuX = 441.0f;
    const float HudElem::s_MenuY = 5.0f;
    const int HudElem::s_MenuWidth = 300;
    const int HudElem::s_MenuHeight = 470;
    const float HudElem::s_Padding = 10.0f;
    const float HudElem::s_TitleHeight = 30.0f;
    const float HudElem::s_LineHeight = 23.0f;

    HudElem::HudElem(int clientNum, float x, float y, const hudelem_color_t& color)
    {
        m_InternalHudElem = HudElem_Alloc(clientNum, 0);
        SetX(x);
        SetY(y);
        SetColor(color);
    }

    void HudElem::SetColor(byte r, byte g, byte b, byte a)
    {
        m_InternalHudElem->elem.color.r = r;
        m_InternalHudElem->elem.color.g = g;
        m_InternalHudElem->elem.color.b = b;
        m_InternalHudElem->elem.color.a = a;
    }
}