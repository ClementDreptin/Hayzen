#include "pch.h"
#include "Core\Menu.h"


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

    m_Feedback = Text("", 50.0f, HudElem::s_MenuY + HudElem::s_MenuHeight - HudElem::s_Padding - 120.0f, HudElem::s_ColorWhite, 0.7f);

    // Initialize the scroller position
    m_iCurrentScrollerPos = 0;

    // Reset the pointer to the bot
    m_pBotEntity = nullptr;

    // Remember that initialization has been done
    m_bInitialized = TRUE;
}

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

VOID Menu::Render()
{
    // If the menu is not initialized, don't go further
    if (!m_bInitialized)
        return;

    // This is a terrible way of getting the feedback text to disappear after
    // three seconds but I didn't find anything else that worked

    // Initialize the timer
    static DWORD dwMilliseconds = 0;

    // Update the feedback text if one has been pushed to the queue
    if (!m_ChangeFeedbackTextQueue.empty())
    {
        m_Feedback.SetText(m_ChangeFeedbackTextQueue.front());
        m_ChangeFeedbackTextQueue.pop();

        // Reset the timer when the feedback text gets set
        dwMilliseconds = 0;
    }

    // Only render the feedback text if it's set
    if (m_Feedback.GetText() != "")
    {
        // We incremement the timer by 16 because the game runs at 60fps with means
        // a frame is rendered every 16 milliseconds (1000 / 60 = 16)
        dwMilliseconds += 16;

        // If more than three seconds have elapsed, reset the timer and the feedback
        // text
        if (dwMilliseconds > 3000)
        {
            m_Feedback.SetText("");
            dwMilliseconds = 0;
        }

        // Only render the feedback text if it's still set (three seconds have not
        // elapsed yet)
        if (m_Feedback.GetText() != "")
            m_Feedback.Draw();
    }

    // If the menu is not open, don't go further
    if (!m_bOpen)
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

VOID Menu::Stop()
{
    // Reset the members
    m_bInitialized = FALSE;
    m_bOpen = FALSE;

    m_SavedPos = vec3(0.0f, 0.0f, 0.0f);
    m_SavedAngles = vec3(0.0f, 0.0f, 0.0f);
    m_bBindsEnabled = FALSE;

    m_pBotEntity = nullptr;
}

VOID Menu::SetFeedbackText(CONST std::string& strText)
{
    // Set the feedback text
    m_ChangeFeedbackTextQueue.push(strText);
}

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

VOID Menu::MoveScroller()
{
    m_Scroller.SetY(HudElem::s_MenuY + HudElem::s_Padding * 2 + HudElem::s_TitleHeight + HudElem::s_LineHeight * m_iCurrentScrollerPos);
}
