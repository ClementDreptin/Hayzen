#include "pch.h"
#include "Games\MW2\TextElem.h"

#include "Games\MW2\Functions.h"

namespace MW2
{
	TextElem::TextElem(int clientNum, const std::string& text, float x, float y, const hudelem_color_t& color, float fontScale)
		: HudElem(clientNum, x, y, color), m_Text(text)
	{
		SetText(text);
		SetFontScale(fontScale);

		m_InternalHudElem->elem.type = HE_TYPE_TEXT;
		m_InternalHudElem->elem.alignOrg = ALIGN_BOTTOM_LEFT;
		m_InternalHudElem->elem.alignScreen = ALIGN_TOP_LEFT;
		m_InternalHudElem->elem.font = 4;
		m_InternalHudElem->elem.sort = 1.0f;
	}
}