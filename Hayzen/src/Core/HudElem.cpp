#include "pch.h"
#include "Core\HudElem.h"

CONST Color HudElem::s_ColorWhite = { 1.0f, 1.0f, 1.0f, 1.0f };
CONST Color HudElem::s_ColorBlack = { 0, 0, 0, 1.0f };
CONST FLOAT HudElem::s_MenuX = 875.0f;
CONST FLOAT HudElem::s_MenuY = 5.0f;
CONST FLOAT HudElem::s_MenuWidth = 400.0f;
CONST FLOAT HudElem::s_MenuHeight = 710.0f;
CONST FLOAT HudElem::s_Padding = 20.0f;
CONST FLOAT HudElem::s_TitleHeight = 60.0f;
CONST FLOAT HudElem::s_LineHeight = 40.0f;

HudElem::HudElem(FLOAT x, FLOAT y, CONST Color& color)
    : m_X(x), m_Y(y), m_Color(color) {}

VOID HudElem::SetColor(FLOAT r, FLOAT g, FLOAT b, FLOAT a)
{
    m_Color.r = r;
    m_Color.g = g;
    m_Color.b = b;
    m_Color.a = a;
}