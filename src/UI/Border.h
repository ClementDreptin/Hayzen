#pragma once

#include "UI/Line.h"

class Border
{
public:
    typedef enum _Position
    {
        Border_None = 0,
        Border_Left = 1 << 0,
        Border_Right = 1 << 1,
        Border_Top = 1 << 2,
        Border_Bottom = 1 << 3,
        Border_All = Border_Left | Border_Right | Border_Top | Border_Bottom,
    } Position;

    struct Props
    {
        float X;
        float Y;
        float Thickness;
        D3DCOLOR Color;
        Position Position;
        float Width;  // Width of the rectangle surrounded by the border.
        float Height; // Height of the rectangle surrounded by the border.
    };

    Border() {}

    void Render(const Props &props);

private:
    Line m_Left;
    Line m_Right;
    Line m_Top;
    Line m_Bottom;
};
