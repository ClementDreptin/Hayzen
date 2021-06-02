#include "pch.h"
#include "Games\MW2\RectangleElem.h"

#include "Games\MW2\Functions.h"

namespace MW2
{
	RectangleElem::RectangleElem(int clientNum, float x, float y, int width, int height, const hudelem_color_t& color)
		: HudElem(clientNum, x, y, color)
	{
		SetWidth(width);
		SetHeight(height);

		m_InternalHudElem->elem.type = HE_TYPE_MATERIAL;
		m_InternalHudElem->elem.alignOrg = ALIGN_TOP_LEFT;
		m_InternalHudElem->elem.alignScreen = ALIGN_TOP_LEFT;
		m_InternalHudElem->elem.sort = 0.0f;
		m_InternalHudElem->elem.materialIndex = G_MaterialIndex("white");
	}
}