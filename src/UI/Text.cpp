#include "pch.h"
#include "UI/Text.h"

#include "UI/Renderer.h"

using namespace Renderer;

void Text::Render(const Props &props)
{
    bool hasBackgroundOrBorder = props.BackgroundColor != 0 || (props.BorderWidth > 0 && props.BorderPosition != Border::Border_None);
    float fontScale = props.FontScale != 0.0f ? props.FontScale : 1.0f;
    float padding = Layout::Padding * fontScale;

    // Render the background if needed
    if (hasBackgroundOrBorder)
    {
        Rectangle::Props rectProps = { 0 };
        rectProps.X = props.X;
        rectProps.Y = props.Y;
        rectProps.Width = GetTextWidth(props.Text, fontScale) + padding * 2;
        rectProps.Height = GetTextHeight(fontScale) + padding * 2;
        rectProps.Color = props.BackgroundColor;
        rectProps.BorderWidth = props.BorderWidth;
        rectProps.BorderColor = props.BorderColor;
        rectProps.BorderPosition = props.BorderPosition;

        m_Background.Render(rectProps);
    }

    // Turn the color into a float array
    float color[4] = {
        ((props.Color & 0x00ff0000) >> 16) / 255.0f,
        ((props.Color & 0x0000ff00) >> 8) / 255.0f,
        ((props.Color & 0x000000ff) >> 0) / 255.0f,
        ((props.Color & 0xff000000) >> 24) / 255.0f,
    };

    // Render the text
    float x = props.X + (hasBackgroundOrBorder ? padding : 0.0f);
    float y = props.Y + (hasBackgroundOrBorder ? padding : 0.0f);
    y += GetTextHeight(fontScale); // The text anchor is at the bottom left instead of top left so we need to add the text height to the Y coordinate
    R_AddCmdDrawText(props.Text.c_str(), props.Text.size(), pFont, x, y, fontScale, fontScale, 0.0f, color, 0);
}
