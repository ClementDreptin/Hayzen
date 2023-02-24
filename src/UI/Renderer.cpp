#include "pch.h"
#include "UI/Renderer.h"

namespace Renderer
{

R_ADDCMDDRAWSTRETCHPIC R_AddCmdDrawStretchPic = nullptr;
R_ADDCMDDRAWTEXT R_AddCmdDrawText = nullptr;
R_TEXTWIDTH R_TextWidth = nullptr;
R_TEXTHEIGHT R_TextHeight = nullptr;
R_REGISTERFONT R_RegisterFont = nullptr;
MATERIAL_REGISTERHANDLE Material_RegisterHandle = nullptr;

Font_s *pFont = nullptr;
HANDLE MaterialHandle = nullptr;

float GetTextWidth(const std::string &text)
{
    return static_cast<float>(R_TextWidth(text.c_str(), text.size(), pFont));
}

float GetTextHeight()
{
    return static_cast<float>(R_TextHeight(pFont));
}

}
