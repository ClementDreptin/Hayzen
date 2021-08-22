#pragma once

struct Color
{
    FLOAT r;
    FLOAT g;
    FLOAT b;
    FLOAT a;
};

class HudElem
{
public:
    static CONST Color s_ColorWhite;
    static CONST Color s_ColorBlack;
    static CONST FLOAT s_MenuX;
    static CONST FLOAT s_MenuY;
    static CONST FLOAT s_MenuWidth;
    static CONST FLOAT s_MenuHeight;
    static CONST FLOAT s_Padding;
    static CONST FLOAT s_TitleHeight;
    static CONST FLOAT s_LineHeight;

    HudElem() {}
    HudElem(FLOAT x, FLOAT y, CONST Color& color);
    virtual ~HudElem() {}

    virtual VOID Draw() CONST = 0;

    FLOAT GetX() CONST { return m_X; }
    FLOAT GetY() CONST { return m_Y; }
    CONST Color& GetColor() CONST { return m_Color; }

    VOID SetX(FLOAT x) { m_X = x; }
    VOID SetY(FLOAT y) { m_Y = y; }
    VOID SetColor(CONST Color& color) { m_Color = color; }
    VOID SetColor(FLOAT r, FLOAT g, FLOAT b, FLOAT a);
    VOID SetAlpha(FLOAT alpha) { m_Color.a = alpha; }
protected:
    FLOAT m_X, m_Y;
    Color m_Color;
};