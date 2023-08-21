#pragma once

#include "UI/Rectangle.h"

class Text
{
public:
    struct Props
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
        Border::Position BorderPosition;
    };

    Text() {}

    void Render(const Props &props);

private:
    Rectangle m_Background;
};
