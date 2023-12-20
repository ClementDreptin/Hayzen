#include "pch.h"
#include "UI/Text.h"

#include "Core/Settings.h"
#include "UI/Renderer.h"

void Text::Render(const Props &props)
{
    bool hasBackgroundOrBorder = props.BackgroundColor != 0 || (props.BorderWidth > 0 && props.BorderPosition != Border::Border_None);
    float fontScale = props.FontScale != 0.0f ? props.FontScale : 1.0f;
    float padding = Settings::Padding * fontScale;

    float textWidth = Renderer::GetTextWidth(props.Text, fontScale);
    float textHeight = Renderer::GetTextHeight(props.Text, fontScale);
    float rectWidth = props.BackgroundWidth != 0.0f ? props.BackgroundWidth : (textWidth + padding * 2);
    float rectHeight = props.BackgroundHeight != 0.0f ? props.BackgroundHeight : (textHeight + padding * 2);

    // Render the background if needed
    if (hasBackgroundOrBorder)
    {
        Rectangle::Props rectProps = {};
        rectProps.X = props.X;
        rectProps.Y = props.Y;
        rectProps.Width = rectWidth;
        rectProps.Height = rectHeight;
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
    float y = props.Y + rectHeight / 2 - textHeight / 2;

    // The text anchor is at the bottom left of the first line (strange decision...)
    // instead of top left so we need to add the text height to the Y coordinate.
    //
    // Example (the anchor is aligned with the underscores (__)):
    //  __ My very cool
    //     multine text
    y += Renderer::GetFontHeight(fontScale);

    Renderer::R_AddCmdDrawText(
        props.Text.c_str(), props.Text.size(),
        Renderer::pFont,
        x, y,
        fontScale, fontScale, 0.0f,
        color,
        0
    );
}
