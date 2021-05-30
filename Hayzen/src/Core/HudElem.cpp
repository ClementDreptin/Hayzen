#include "pch.h"
#include "Core\HudElem.h"

const Color HudElem::s_ColorWhite = { 1.0f, 1.0f, 1.0f, 1.0f };
const Color HudElem::s_ColorWhiteNoAlpha = { 1.0f, 1.0f, 1.0f, 0.0f };
const Color HudElem::s_ColorBlack = { 0, 0, 0, 1.0f };
const Color HudElem::s_ColorBlackNoAlpha = { 0, 0, 0, 0 };

HudElem::HudElem(float x, float y, const Color& color)
	: m_X(x), m_Y(y), m_Color(color) {}

void HudElem::MakeAppear(float alpha)
{
	m_Color.a = alpha;
}

void HudElem::MakeDisappear()
{
	m_Color.a = 0;
}