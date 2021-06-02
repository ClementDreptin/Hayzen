#pragma once

struct Color
{
	float r;
	float g;
	float b;
	float a;
};

class HudElem
{
public:
	static const Color s_ColorWhite;
	static const Color s_ColorBlack;
	static const float s_MenuX;
	static const float s_MenuY;
	static const float s_MenuWidth;
	static const float s_MenuHeight;
	static const float s_Padding;
	static const float s_TitleHeight;
	static const float s_LineHeight;

	HudElem() {}
	HudElem(float x, float y, const Color& color);
	virtual ~HudElem() {}

	virtual void Draw() const = 0;

	float GetX() const { return m_X; }
	float GetY() const { return m_Y; }
	const Color& GetColor() const { return m_Color; }

	void SetX(float x) { m_X = x; }
	void SetY(float y) { m_Y = y; }
	void SetColor(const Color& color) { m_Color = color; }
	void SetColor(float r, float g, float b, float a);
	void SetAlpha(float alpha) { m_Color.a = alpha; }
protected:
	float m_X, m_Y;
	Color m_Color;
};