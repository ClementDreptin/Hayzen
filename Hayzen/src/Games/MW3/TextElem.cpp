#include "pch.h"
#include "Games\MW3\TextElem.h"

#include "Games\MW3\Functions.h"

namespace MW3
{
    TextElem::TextElem(INT clientNum, CONST std::string& text, FLOAT x, FLOAT y, CONST hudelem_color_t& color, FLOAT fontScale)
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