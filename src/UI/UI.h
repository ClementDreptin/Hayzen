#pragma once

namespace UI
{

struct LineProps
{
    float X;
    float Y;
    float Width;
    float Height;
    D3DCOLOR Color;
};

void DrawLine(const LineProps &props);

typedef enum _BorderPosition
{
    Border_None = 0,
    Border_Left = 1 << 0,
    Border_Right = 1 << 1,
    Border_Top = 1 << 2,
    Border_Bottom = 1 << 3,
    Border_All = Border_Left | Border_Right | Border_Top | Border_Bottom,
} BorderPosition;

struct BorderProps
{
    float X;
    float Y;
    float Thickness;
    D3DCOLOR Color;
    BorderPosition Position;
    float Width;  // Width of the rectangle surrounded by the border.
    float Height; // Height of the rectangle surrounded by the border.
};

void DrawBorder(const BorderProps &props);

struct RectangleProps
{
    float X;
    float Y;
    float Width;
    float Height;
    D3DCOLOR Color;
    float BorderWidth;
    D3DCOLOR BorderColor;
    BorderPosition BorderPosition;
};

void DrawRectangle(const RectangleProps &props);

struct TextProps
{
    float X;
    float Y;
    std::string Text;
    float FontScale;
    D3DCOLOR Color;
    float BackgroundWidth;  // Use this if you want a different width than the one calculated to fit the string
    float BackgroundHeight; // Use this if you want a different height than the one calculated to fit the string
    D3DCOLOR BackgroundColor;
    float BorderWidth;
    D3DCOLOR BorderColor;
    BorderPosition BorderPosition;
};

void DrawText(const TextProps &props);

}
