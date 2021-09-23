#include "pch.h"
#include "Core\Menu.h"


//--------------------------------------------------------------------------------------
// Name: Init()
// Desc: Create the constant HUD elements.
//--------------------------------------------------------------------------------------
VOID Menu::Init(INT iClientNum, CONST Structure& structure)
{
    // Save the arguments to class members
    m_iClientNum = iClientNum;
    m_Structure = structure;

    // Create the constant HUD elemùents
    m_Background = Rectangle(HudElem::s_MenuX, HudElem::s_MenuY, HudElem::s_MenuWidth, HudElem::s_MenuHeight, HudElem::s_ColorBlack);
    m_Background.SetAlpha(0.7f);

    m_Title = Text("Cod Jumper", HudElem::s_MenuX + HudElem::s_Padding, HudElem::s_Padding + HudElem::s_TitleHeight, HudElem::s_ColorWhite, 1.7f);

    m_Scroller = Rectangle(HudElem::s_MenuX, HudElem::s_MenuY + (HudElem::s_Padding * 2) + HudElem::s_TitleHeight, HudElem::s_MenuWidth, HudElem::s_LineHeight, HudElem::s_ColorWhite);
    m_Scroller.SetAlpha(0.7f);

    m_Instructions = Text("Navigate: UP - DOWN | Select: X | Back: RS",
        HudElem::s_MenuX + HudElem::s_Padding, HudElem::s_MenuY + HudElem::s_MenuHeight - HudElem::s_Padding - 80, HudElem::s_ColorWhite, 0.7f);

    // Remember that initialization has been done
    m_bInitialized = TRUE;
}


//--------------------------------------------------------------------------------------
// Name: Update()
// Desc: Listen for controller inputs and update the menu accordingly.
//--------------------------------------------------------------------------------------
VOID Menu::Update()
{
    // If the menu is not initialized, don't go further
    if (!m_bInitialized)
        return;

    // Call XInputGetState
    // Update the cursor position
    // Call OnClick on the selected option
}


//--------------------------------------------------------------------------------------
// Name: Render()
// Desc: Render every HUD element.
//--------------------------------------------------------------------------------------
VOID Menu::Render()
{
    // If the menu is not initialized, don't go further
    if (!m_bInitialized)
        return;

    // Draw the constant HUD elements
    m_Background.Draw();
    m_Title.Draw();
    m_Scroller.Draw();
    m_Instructions.Draw();

    // Draw every option in the current menu section
    for (size_t i = 0; i < m_Structure[m_Title.GetText()].size(); i++)
        m_Structure[m_Title.GetText()][i].Draw();
}


//--------------------------------------------------------------------------------------
// Name: Stop()
// Desc: Stop the menu.
//--------------------------------------------------------------------------------------
VOID Menu::Stop()
{
    m_bInitialized = FALSE;
}
