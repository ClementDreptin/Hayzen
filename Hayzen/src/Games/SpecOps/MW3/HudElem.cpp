#include "pch.h"
#include "Games\SpecOps\MW3\HudElem.h"

#include "Games\SpecOps\MW3\Functions.h"

namespace SpecOps
{
namespace MW3
{
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
}
}