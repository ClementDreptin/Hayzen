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
	static const Color s_ColorWhiteNoAlpha;
	static const Color s_ColorBlack;
	static const Color s_ColorBlackNoAlpha;

	HudElem() {}
	HudElem(float x, float y, const Color& color);
	virtual ~HudElem() {}

	virtual void Draw() const = 0;

	void MakeAppear(float alpha = 1.0f);
	void MakeDisappear();

	float GetX() const { return m_X; }
	float GetY() const { return m_Y; }
	const Color& GetColor() const { return m_Color; }

	void SetX(float x) { m_X = x; }
	void SetY(float y) { m_Y = y; }
	void SetColor(const Color& color) { m_Color = color; }
protected:
	float m_X, m_Y;
	Color m_Color;
};