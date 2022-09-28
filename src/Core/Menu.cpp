#include "pch.h"
#include "Core\Menu.h"

void Menu::Init(int clientNum, Option *pOption)
{
    // Save the arguments to class members
    m_ClientNum = clientNum;
    m_pCurrentOption = pOption;

    // Create the constant HUD elements
    m_Background = Rectangle(HudElem::s_MenuX, HudElem::s_MenuY, HudElem::s_MenuWidth, HudElem::s_MenuHeight, HudElem::s_ColorBlack);
    m_Background.SetAlpha(0.7f);

    m_Title = Text(m_pCurrentOption->GetText(), HudElem::s_MenuX + HudElem::s_Padding, HudElem::s_Padding + HudElem::s_TitleHeight, HudElem::s_ColorWhite, 1.7f);

    m_Scroller = Rectangle(HudElem::s_MenuX, HudElem::s_MenuY + (HudElem::s_Padding * 2) + HudElem::s_TitleHeight, HudElem::s_MenuWidth, HudElem::s_LineHeight, HudElem::s_ColorWhite);
    m_Scroller.SetAlpha(0.7f);

    m_Instructions = Text("Navigate: " CHAR_UP " - " CHAR_DOWN " | Select: " CHAR_X " | Back: " CHAR_RS, HudElem::s_MenuX + HudElem::s_Padding, HudElem::s_MenuY + HudElem::s_MenuHeight - HudElem::s_Padding - 80.0f, HudElem::s_ColorWhite, 0.7f);

    // Initialize the scroller position
    m_CurrentScrollerPos = 0;

    // Reset the pointer to the bot
    m_pBotEntity = nullptr;

    // Remember that initialization has been done
    m_Initialized = true;
}

void Menu::Update()
{
    // If the menu is not initialized, don't go further
    if (!m_Initialized)
        return;

    // Get the current gamepad state
    XINPUT_STATE inputState;
    XInputGetState(0, &inputState);

    // Save the buttons pressed at the previous frame to set the currently pressed buttons only if
    // they were not already pressed at the previous frame, we need to do this because pressing
    // then releasing a button (even done really fast) takes multiple frames.
    static uint16_t lastButtons = 0;
    uint16_t pressedButtons = (lastButtons ^ inputState.Gamepad.wButtons) & inputState.Gamepad.wButtons;
    lastButtons = inputState.Gamepad.wButtons;

    // Open/Close the menu
    if (pressedButtons & XINPUT_GAMEPAD_DPAD_LEFT)
        m_Open = !m_Open;

    // Allow the user to select options with the DPAD only when the menu is open
    if (pressedButtons & XINPUT_GAMEPAD_DPAD_UP && m_Open)
    {
        m_CurrentScrollerPos--;

        // If the scroller is already at the top, send it to the bottom
        if (m_CurrentScrollerPos < 0)
            m_CurrentScrollerPos = static_cast<int>(m_pCurrentOption->GetChildren().size()) - 1;

        MoveScroller();
    }

    if (pressedButtons & XINPUT_GAMEPAD_DPAD_DOWN && m_Open)
    {
        m_CurrentScrollerPos++;

        // If the scroller is already at the bottom, send it to the top
        if (m_CurrentScrollerPos >= static_cast<int>(m_pCurrentOption->GetChildren().size()))
            m_CurrentScrollerPos = 0;

        MoveScroller();
    }

    // Allow the user to click on an option
    if (pressedButtons & XINPUT_GAMEPAD_X && m_Open)
        m_pCurrentOption->GetChildren()[m_CurrentScrollerPos]->OnClick(this);

    // Allow the user to go back to the previous menu section
    if (pressedButtons & XINPUT_GAMEPAD_RIGHT_THUMB && m_Open)
    {
        // We can't just update the current option right away because its children
        // might still be rendering. That's why we push the new option to a queue
        // to update it later once it's safe.
        if (m_pCurrentOption->HasParent())
            m_ChangeSectionQueue.push(m_pCurrentOption->GetParent());
    }

    // Call the current load position function if the save and load binds are enabled
    if (pressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER && m_BindsEnabled)
        m_LoadPositionFn(this);

    // Call the current save position function if the save and load binds are enabled
    if (pressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER && m_BindsEnabled)
        m_SavePositionFn(this);
}

void Menu::Render()
{
    // If the menu is not initialized or not open, don't go further
    if (!m_Initialized || !m_Open)
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

void Menu::Stop()
{
    // Reset the members
    m_Initialized = false;
    m_Open = false;

    m_SavedPosition = vec3(0.0f, 0.0f, 0.0f);
    m_SavedAngles = vec3(0.0f, 0.0f, 0.0f);
    m_BindsEnabled = false;

    m_pBotEntity = nullptr;
}

void Menu::SetCurrentOption(Option *pOption)
{
    // Reset the scroller position
    m_CurrentScrollerPos = 0;
    MoveScroller();

    // Set the current option
    m_pCurrentOption = pOption;

    // Update the title accordingly
    m_Title.SetText(m_pCurrentOption->GetText());
}

void Menu::MoveScroller()
{
    m_Scroller.SetY(HudElem::s_MenuY + HudElem::s_Padding * 2 + HudElem::s_TitleHeight + HudElem::s_LineHeight * m_CurrentScrollerPos);
}
