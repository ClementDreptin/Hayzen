#include "pch.h"
#include "Elements\HudElem.h"


CONST Color HudElem::s_ColorWhite = { 1.0f, 1.0f, 1.0f, 1.0f };
CONST Color HudElem::s_ColorBlack = { 0.0f, 0.0f, 0.0f, 1.0f };
CONST FLOAT HudElem::s_MenuX = 875.0f;
CONST FLOAT HudElem::s_MenuY = 5.0f;
CONST FLOAT HudElem::s_MenuWidth = 400.0f;
CONST FLOAT HudElem::s_MenuHeight = 710.0f;
CONST FLOAT HudElem::s_Padding = 20.0f;
CONST FLOAT HudElem::s_TitleHeight = 60.0f;
CONST FLOAT HudElem::s_LineHeight = 40.0f;
Font_s* HudElem::s_pFont = nullptr;
HANDLE HudElem::s_hMaterial = nullptr;
R_REGISTERFONT HudElem::R_RegisterFont = nullptr;
MATERIAL_REGISTERHANDLE HudElem::Material_RegisterHandle = nullptr;
R_ADDCMDDRAWTEXT HudElem::R_AddCmdDrawText = nullptr;
R_ADDCMDDRAWSTRETCHPIC HudElem::R_AddCmdDrawStretchPic = nullptr;


HudElem::HudElem(FLOAT fX, FLOAT fY, CONST Color& color)
    : m_fX(fX), m_fY(fY), m_Color(color) {}

VOID HudElem::SetColor(FLOAT fR, FLOAT fG, FLOAT fB, FLOAT fA)
{
    m_Color.r = fR;
    m_Color.g = fG;
    m_Color.b = fB;
    m_Color.a = fA;
}
