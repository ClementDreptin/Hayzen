#include "pch.h"
#include "UI/Text.h"

#include "UI/Renderer.h"

using namespace Renderer;

void Text::Render(const Props &props)
{
    bool hasBackgroundOrBorder = props.BackgroundColor != 0 || (props.BorderWidth > 0 && props.BorderPosition != Border::Border_None);

    // Render the background if needed
    if (hasBackgroundOrBorder)
    {
        Rectangle::Props rectProps = { 0 };
        rectProps.X = props.X;
        rectProps.Y = props.Y;
        rectProps.Width = R_TextWidth(props.Text.c_str(), props.Text.size(), pFont) + Layout::Padding * 2;
        rectProps.Height = Layout::LineHeight;
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
    float x = props.X + (hasBackgroundOrBorder ? Layout::Padding : 0.0f);
    float y = props.Y + R_TextHeight(pFont) + (hasBackgroundOrBorder ? Layout::Padding : 0.0f);
    R_AddCmdDrawText(props.Text.c_str(), props.Text.size(), pFont, x, y, 1.0f, 1.0f, 0.0f, color, 0);
}
