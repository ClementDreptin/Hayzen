#include "pch.h"
#include "Core\Menu.h"


//--------------------------------------------------------------------------------------
// Name: Init()
// Desc: Create the constant HUD elements.
//--------------------------------------------------------------------------------------
VOID Menu::Init(INT iClientNum, Option* pOption)
{
    // Save the arguments to class members
    m_iClientNum = iClientNum;
    m_pCurrentOption = pOption;

    // Create the constant HUD elements
    m_Background = Rectangle(HudElem::s_MenuX, HudElem::s_MenuY, HudElem::s_MenuWidth, HudElem::s_MenuHeight, HudElem::s_ColorBlack);
    m_Background.SetAlpha(0.7f);

    m_Title = Text(m_pCurrentOption->GetText(), HudElem::s_MenuX + HudElem::s_Padding, HudElem::s_Padding + HudElem::s_TitleHeight, HudElem::s_ColorWhite, 1.7f);

    m_Scroller = Rectangle(HudElem::s_MenuX, HudElem::s_MenuY + (HudElem::s_Padding * 2) + HudElem::s_TitleHeight, HudElem::s_MenuWidth, HudElem::s_LineHeight, HudElem::s_ColorWhite);
    m_Scroller.SetAlpha(0.7f);

    m_Instructions = Text("Navigate: " CHAR_UP " - " CHAR_DOWN " | Select: " CHAR_X " | Back: " CHAR_RS,
        HudElem::s_MenuX + HudElem::s_Padding, HudElem::s_MenuY + HudElem::s_MenuHeight - HudElem::s_Padding - 80.0f, HudElem::s_ColorWhite, 0.7f);

    // Initialize the scroller position
    m_iCurrentScrollerPos = 0;

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

    // Get the current gamepad state
    XINPUT_STATE InputState;
    XInputGetState(0, &InputState);

    // Save the buttons pressed at the previous frame to set the currently pressed buttons only if
    // they were not already pressed at the previous frame, we need to do this because pressing
    // then releasing a button (even done really fast) takes multiple frames.
    static WORD wLastButtons = 0;
    WORD wPressedButtons = (wLastButtons ^ InputState.Gamepad.wButtons) & InputState.Gamepad.wButtons;
    wLastButtons = InputState.Gamepad.wButtons;

    // Open/Close the menu
    if (wPressedButtons & XINPUT_GAMEPAD_DPAD_LEFT)
        m_bOpen = !m_bOpen;

    // Allow the user to select options with the DPAD only when the menu is open
    if (wPressedButtons & XINPUT_GAMEPAD_DPAD_UP && m_bOpen)
    {
        m_iCurrentScrollerPos--;

        // If the scroller is already at the top, send it to the bottom
        if (m_iCurrentScrollerPos < 0)
            m_iCurrentScrollerPos = m_pCurrentOption->GetChildren().size() - 1;

        MoveScroller();
    }

    if (wPressedButtons & XINPUT_GAMEPAD_DPAD_DOWN && m_bOpen)
    {
        m_iCurrentScrollerPos++;

        // If the scroller is already at the bottom, send it to the top
        if (m_iCurrentScrollerPos >= (INT)m_pCurrentOption->GetChildren().size())
            m_iCurrentScrollerPos = 0;

        MoveScroller();
    }

    // Allow the user to click on an option
    if (wPressedButtons & XINPUT_GAMEPAD_X && m_bOpen)
        m_pCurrentOption->GetChildren()[m_iCurrentScrollerPos]->OnClick(this);

    // Allow the user to go back to the previous menu section
    if (wPressedButtons & XINPUT_GAMEPAD_RIGHT_THUMB && m_bOpen)
    {
        // We can't just update the current option right away because its children
        // might still be rendering. That's why we push the new option to a queue
        // to update it later once it's safe.
        if (m_pCurrentOption->HasParent())
            m_ChangeSectionQueue.push(m_pCurrentOption->GetParent());
    }

    // Call the current load position function if the save and load binds are enabled
    if (wPressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER && m_bBindsEnabled)
        m_fnLoadPosition(this);

    // Call the current save position function if the save and load binds are enabled
    if (wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER && m_bBindsEnabled)
        m_fnSavePosition(this);
}


//--------------------------------------------------------------------------------------
// Name: Render()
// Desc: Render every HUD element.
//--------------------------------------------------------------------------------------
VOID Menu::Render()
{
    // If the menu is not initialized or not open, don't go further
    if (!m_bInitialized || !m_bOpen)
        return;

    // Draw the constant HUD elements
    m_Background.Draw();
    m_Title.Draw();
    m_Scroller.Draw();
    m_Instructions.Draw();

    // Update the current option if one has been pushed to the queue
    if (!m_ChangeSectionQueue.empty())
    {
        SetCurrentOption(m_ChangeSectionQueue.front());
        m_ChangeSectionQueue.pop();
    }

    // Draw every option in the current menu section
    for (size_t i = 0; i < m_pCurrentOption->GetChildren().size(); i++)
        m_pCurrentOption->GetChildren()[i]->Draw();
}


//--------------------------------------------------------------------------------------
// Name: Stop()
// Desc: Stop the menu.
//--------------------------------------------------------------------------------------
VOID Menu::Stop()
{
    m_bInitialized = FALSE;
}


//--------------------------------------------------------------------------------------
// Name: SetCurrentOption()
// Desc: Change the current menu section.
//--------------------------------------------------------------------------------------
VOID Menu::SetCurrentOption(Option* pOption)
{
    // Reset the scroller position
    m_iCurrentScrollerPos = 0;
    MoveScroller();

    // Set the current option
    m_pCurrentOption = pOption;

    // Update the title accordingly
    m_Title.SetText(m_pCurrentOption->GetText());
}


//--------------------------------------------------------------------------------------
// Name: MoveScroller()
// Desc: Update the poition of the scroller according to the m_iCurrentScrollerPos
//       index.
//--------------------------------------------------------------------------------------
VOID Menu::MoveScroller()
{
    m_Scroller.SetY(HudElem::s_MenuY + HudElem::s_Padding * 2 + HudElem::s_TitleHeight + HudElem::s_LineHeight * m_iCurrentScrollerPos);
}
