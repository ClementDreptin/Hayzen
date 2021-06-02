#pragma once

#include "Games\MW2\HudElem.h"
#include "Games\MW2\Functions.h"

namespace MW2
{
	class TextElem : public HudElem
	{
	public:
		TextElem() {}
		TextElem(int clientNum, const std::string& text, float x, float y, const hudelem_color_t& color, float fontScale = 1.0f);

		float GetFontScale() const { return m_InternalHudElem->elem.fontScale; }
		const std::string& GetText() const { return m_Text; }

		void SetFontScale(float fontScale) { m_InternalHudElem->elem.fontScale = fontScale; }
		void SetText(const std::string& text) { m_Text = text; m_InternalHudElem->elem.text = G_LocalizedStringIndex(text.c_str()); }
	private:
		std::string m_Text;
	};
}