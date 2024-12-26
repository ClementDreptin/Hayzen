#include "pch.h"
#include "Core/UI.h"

#include "Core/Settings.h"

namespace UI
{

void DrawLine(const LineProps &props)
{
    XASSERT(R_AddCmdDrawStretchPic != nullptr);

    // Turn the color into a float array
    float color[4] = {
        ((props.Color & 0x00ff0000) >> 16) / 255.0f,
        ((props.Color & 0x0000ff00) >> 8) / 255.0f,
        ((props.Color & 0x000000ff) >> 0) / 255.0f,
        ((props.Color & 0xff000000) >> 24) / 255.0f,
    };

    // Render the line
    R_AddCmdDrawStretchPic(
        props.X, props.Y,
        props.Width, props.Height,
        0.0f, 0.0f, 1.0f, 1.0f,
        color,
        MaterialHandle
    );
}

void DrawBorder(const BorderProps &props)
{
    // Return early if no borders need to be rendered
    if (props.Position == Border_None || props.Thickness == 0)
        return;

    // Render the left border if needed
    if (props.Position & Border_Left)
    {
        LineProps lineProps = {};
        lineProps.X = props.X - props.Thickness;
        lineProps.Y = props.Y;
        lineProps.Width = props.Thickness;
        lineProps.Height = props.Height + (props.Position & Border_Bottom ? props.Thickness : 0.0f);
        lineProps.Color = props.Color;

        DrawLine(lineProps);
    }

    // Render the right border if needed
    if (props.Position & Border_Right)
    {
        LineProps lineProps = {};
        lineProps.X = props.X + props.Width;
        lineProps.Y = props.Y - (props.Position & Border_Top ? props.Thickness : 0.0f);
        lineProps.Width = props.Thickness;
        lineProps.Height = props.Height + (props.Position & Border_Top ? props.Thickness : 0.0f);
        lineProps.Color = props.Color;

        DrawLine(lineProps);
    }

    // Render the top border if needed
    if (props.Position & Border_Top)
    {
        LineProps lineProps = {};
        lineProps.X = props.X - (props.Position & Border_Left ? props.Thickness : 0.0f);
        lineProps.Y = props.Y - props.Thickness;
        lineProps.Width = props.Width + (props.Position & Border_Left ? props.Thickness : 0.0f);
        lineProps.Height = props.Thickness;
        lineProps.Color = props.Color;

        DrawLine(lineProps);
    }

    // Render the bottom border if needed
    if (props.Position & Border_Bottom)
    {
        LineProps lineProps = {};
        lineProps.X = props.X;
        lineProps.Y = props.Y + props.Height;
        lineProps.Width = props.Width + (props.Position & Border_Right ? props.Thickness : 0.0f);
        lineProps.Height = props.Thickness;
        lineProps.Color = props.Color;

        DrawLine(lineProps);
    }
}

void DrawRectangle(const RectangleProps &props)
{
    XASSERT(R_AddCmdDrawStretchPic != nullptr);

    bool hasBorder = props.BorderPosition != Border_None && props.BorderWidth > 0;

    // Turn the color into a float array
    float color[4] = {
        ((props.Color & 0x00ff0000) >> 16) / 255.0f,
        ((props.Color & 0x0000ff00) >> 8) / 255.0f,
        ((props.Color & 0x000000ff) >> 0) / 255.0f,
        ((props.Color & 0xff000000) >> 24) / 255.0f,
    };

    // Render the rectangle
    R_AddCmdDrawStretchPic(
        props.X, props.Y,
        props.Width, props.Height,
        0.0f, 0.0f, 1.0f, 1.0f,
        color,
        MaterialHandle
    );

    // Render the border if needed
    if (hasBorder)
    {
        BorderProps borderProps = {};
        borderProps.X = props.X;
        borderProps.Y = props.Y;
        borderProps.Thickness = props.BorderWidth;
        borderProps.Color = props.BorderColor;
        borderProps.Position = props.BorderPosition;
        borderProps.Width = props.Width;
        borderProps.Height = props.Height;

        DrawBorder(borderProps);
    }
}

void DrawText(const TextProps &props)
{
    XASSERT(R_AddCmdDrawText != nullptr);

    bool hasBackgroundOrBorder = props.BackgroundColor != 0 || (props.BorderWidth > 0 && props.BorderPosition != Border_None);
    float fontScale = props.FontScale != 0.0f ? props.FontScale : 1.0f;
    float padding = Settings::Padding * fontScale;

    float textWidth = GetTextWidth(props.Text, fontScale);
    float textHeight = GetTextHeight(props.Text, fontScale);
    float rectWidth = props.BackgroundWidth != 0.0f ? props.BackgroundWidth : (textWidth + padding * 2);
    float rectHeight = props.BackgroundHeight != 0.0f ? props.BackgroundHeight : (textHeight + padding * 2);

    // Render the background if needed
    if (hasBackgroundOrBorder)
    {
        RectangleProps rectProps = {};
        rectProps.X = props.X;
        rectProps.Y = props.Y;
        rectProps.Width = rectWidth;
        rectProps.Height = rectHeight;
        rectProps.Color = props.BackgroundColor;
        rectProps.BorderWidth = props.BorderWidth;
        rectProps.BorderColor = props.BorderColor;
        rectProps.BorderPosition = props.BorderPosition;

        DrawRectangle(rectProps);
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
    y += GetFontHeight(fontScale);

    R_AddCmdDrawText(
        props.Text.c_str(), props.Text.size(),
        pFont,
        x, y,
        fontScale, fontScale, 0.0f,
        color,
        0
    );
}

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
    XASSERT(R_TextWidth != nullptr);

    return static_cast<float>(R_TextWidth(text.c_str(), text.size(), pFont)) * fontScale;
}

float GetTextHeight(const std::string &text, float fontScale)
{
    size_t numberOfLines = std::count(text.begin(), text.end(), '\n') + 1;

    return GetFontHeight(fontScale) * numberOfLines;
}

float GetFontHeight(float fontScale)
{
    XASSERT(R_TextHeight != nullptr);

    return static_cast<float>(R_TextHeight(pFont)) * fontScale;
}

}
