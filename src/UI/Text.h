#pragma once

#include "UI/Layout.h"
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
