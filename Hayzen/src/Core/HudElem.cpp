#include "pch.h"
#include "Core\HudElem.h"

const Color HudElem::s_ColorWhite = { 1.0f, 1.0f, 1.0f, 1.0f };
const Color HudElem::s_ColorBlack = { 0, 0, 0, 1.0f };
const float HudElem::s_MenuX = 875.0f;
const float HudElem::s_MenuY = 5.0f;
const float HudElem::s_MenuWidth = 400.0f;
const float HudElem::s_MenuHeight = 710.0f;
const float HudElem::s_Padding = 20.0f;
const float HudElem::s_TitleHeight = 60.0f;
const float HudElem::s_LineHeight = 40.0f;

HudElem::HudElem(float x, float y, const Color& color)
	: m_X(x), m_Y(y), m_Color(color) {}

void HudElem::SetColor(float r, float g, float b, float a)
{
	m_Color.r = r;
	m_Color.g = g;
	m_Color.b = b;
	m_Color.a = a;
}