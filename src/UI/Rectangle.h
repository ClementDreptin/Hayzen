#pragma once

#include "UI/Border.h"

class Rectangle
{
public:
    struct Props
    {
        float X;
        float Y;
        float Width;
        float Height;
        D3DCOLOR Color;
        float BorderWidth;
        D3DCOLOR BorderColor;
        Border::Position BorderPosition;
    };

    Rectangle() {}

    void Render(const Props &props);

private:
    Border m_Border;
};
