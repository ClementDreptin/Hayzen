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

// The resolution is always 720p, 1080p is created by the hardware scaler
float DisplayWidth = 1280.0f;
float DisplayHeight = 720.0f;

float GetTextWidth(const std::string &text, float fontScale)
{
    return static_cast<float>(R_TextWidth(text.c_str(), text.size(), pFont)) * fontScale;
}

float GetTextHeight(const std::string &text, float fontScale)
{
    size_t numberOfLines = std::count(text.begin(), text.end(), '\n') + 1;

    return GetFontHeight() * numberOfLines;
}

float GetFontHeight(float fontScale)
{
    return static_cast<float>(R_TextHeight(pFont)) * fontScale;
}

}
