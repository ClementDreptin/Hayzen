#include "pch.h"
#include "Games\Alpha\MW2\RectangleElem.h"

#include "Games\Alpha\MW2\Functions.h"

namespace Alpha
{
namespace MW2
{
	RectangleElem::RectangleElem(float x, float y, float width, float height, const Color& color)
		: HudElem(x, y, color), m_Width(width), m_Height(height)
	{
		m_MaterialHandle = Material_RegisterHandle("white", 0);
	}

	void RectangleElem::Draw() const
	{
		if (m_Color.a > 0.0f)
			R_AddCmdDrawStretchPic(m_X, m_Y, m_Width, m_Height, 0.0f, 0.0f, 1.0f, 1.0f, (float*)&m_Color, m_MaterialHandle);
	}
}
}