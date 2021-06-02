#include "pch.h"
#include "Games\MW2\TextElem.h"

#include "Games\MW2\Functions.h"

namespace MW2
{
	TextElem::TextElem(const std::string& text, float x, float y, const Color& color, float fontScale)
		: HudElem(x, y, color), m_FontScale(fontScale), m_Text(text)
	{
		m_Font = R_RegisterFont("fonts/normalFont", 0);
	}

	void TextElem::Draw() const
	{
		if (m_Color.a > 0.0f)
			R_AddCmdDrawText(m_Text.c_str(), strlen(m_Text.c_str()), m_Font, m_X, m_Y, m_FontScale, m_FontScale, 0, (float*)&m_Color, 0);
	}
}