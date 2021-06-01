#pragma once

#include "Core\HudElem.h"
#include "Games\Alpha\MW2\Structs.h"

namespace Alpha
{
namespace MW2
{
	class TextElem : public HudElem
	{
	public:
		TextElem() {}
		TextElem(const std::string& text, float x, float y, const Color& color, float fontScale = 1.0f);

		virtual void Draw() const;

		float GetFontScale() const { return m_FontScale; }
		const std::string& GetText() const { return m_Text; }

		void SetFontScale(float fontScale) { m_FontScale = fontScale; }
		void SetText(const std::string& text) { m_Text = text; }
	private:
		Font_s* m_Font;
		float m_FontScale;
		std::string m_Text;
	};
}
}