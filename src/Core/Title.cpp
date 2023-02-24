#include "pch.h"
#include "Core/Title.h"

#include "Core/Input.h"
#include "UI/Renderer.h"

Detour *Title::s_pSCR_DrawScreenFieldDetour = nullptr;
Title *Title::s_CurrentInstance = nullptr;

Title::Title()
    : m_InMatch(false), m_MenuOpen(false)
{
    s_CurrentInstance = this;
}

Title::~Title()
{
    delete s_pSCR_DrawScreenFieldDetour;
}

void Title::Update()
{
    // Get the current gamepad state
    Input::Gamepad *pGamepad = Input::GetInput();

    // Toggle the menu by pressing LT and DPAD LEFT
    if (pGamepad->LastLeftTrigger && pGamepad->PressedButtons & XINPUT_GAMEPAD_DPAD_LEFT)
    {
        m_MenuOpen = !m_MenuOpen;
        return;
    }

    // Update the menu if it's open
    if (m_MenuOpen)
        m_Menu.Update(pGamepad);
}

void Title::Render()
{
    // Render the menu if it's open
    if (m_MenuOpen)
        m_Menu.Render();

    // Render the controls text
    RenderControlsText();
}

void Title::RenderControlsText()
{
    Text::Props props = { 0 };
    props.X = 10.0f;
    props.Y = 10.0f;
    props.Text = "Hold " CHAR_LT " & press " CHAR_LEFT " to " + std::string(!m_MenuOpen ? "Open" : "Close");
    props.Color = Layout::TextColor;
    props.BackgroundColor = Layout::BackgroundColor;
    props.BorderWidth = Layout::BorderWidth;
    props.BorderColor = Layout::Color;
    props.BorderPosition = Border::Border_All;

    m_ControlsText.Render(props);
}

void Title::SCR_DrawScreenFieldHook(const int localClientNum, int refreshedUI)
{
    // Call the original SCR_DrawScreenField function
    s_pSCR_DrawScreenFieldDetour->GetOriginal<decltype(&SCR_DrawScreenFieldHook)>()(localClientNum, refreshedUI);

    if (s_CurrentInstance != nullptr && s_CurrentInstance->InMatch())
    {
        s_CurrentInstance->Update();

        s_CurrentInstance->Render();
    }
}

void Title::InitRenderer()
{
    using namespace Renderer;

    pFont = R_RegisterFont("fonts/normalFont", 0);
    MaterialHandle = Material_RegisterHandle("white", 0);

    Layout::LineHeight = R_TextHeight(pFont) + Layout::Padding * 2;
}
