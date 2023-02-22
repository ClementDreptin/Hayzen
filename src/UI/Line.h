#pragma once

class Line
{
public:
    struct Props
    {
        float X;
        float Y;
        float Width;
        float Height;
        D3DCOLOR Color;
    };

    Line() {}

    void Render(const Props &props);
};
