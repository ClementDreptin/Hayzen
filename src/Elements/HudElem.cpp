#include "pch.h"
#include "Elements\HudElem.h"


const Color HudElem::s_ColorWhite = { 1.0f, 1.0f, 1.0f, 1.0f };
const Color HudElem::s_ColorBlack = { 0.0f, 0.0f, 0.0f, 1.0f };
const float HudElem::s_MenuX = 875.0f;
const float HudElem::s_MenuY = 5.0f;
const float HudElem::s_MenuWidth = 400.0f;
const float HudElem::s_MenuHeight = 710.0f;
const float HudElem::s_Padding = 20.0f;
const float HudElem::s_TitleHeight = 60.0f;
const float HudElem::s_LineHeight = 40.0f;
Font_s *HudElem::s_pFont = nullptr;
HANDLE HudElem::s_hMaterial = nullptr;
R_REGISTERFONT HudElem::R_RegisterFont = nullptr;
MATERIAL_REGISTERHANDLE HudElem::Material_RegisterHandle = nullptr;
R_ADDCMDDRAWTEXT HudElem::R_AddCmdDrawText = nullptr;
R_ADDCMDDRAWSTRETCHPIC HudElem::R_AddCmdDrawStretchPic = nullptr;


HudElem::HudElem(float fX, float fY, const Color &color)
    : m_fX(fX), m_fY(fY), m_Color(color) {}

void HudElem::SetColor(float fR, float fG, float fB, float fA)
{
    m_Color.r = fR;
    m_Color.g = fG;
    m_Color.b = fB;
    m_Color.a = fA;
}
