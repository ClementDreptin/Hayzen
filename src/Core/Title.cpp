#include "pch.h"
#include "Core/Title.h"

#include "Core/Context.h"
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

    // Save and Load with LB/RB when Save and Load binds are enabled
    if (Context::BindsEnabled && !m_MenuOpen)
    {
        if (pGamepad->PressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
            Context::LoadPositionFn(nullptr);
        else if (pGamepad->PressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
            Context::SavePositionFn(nullptr);
    }
}

void Title::Render()
{
    // Render the menu if it's open
    if (m_MenuOpen)
        m_Menu.Render();

    // Render the controls text if enabled
    if (Context::DisplayControlsTexts)
        RenderControlsTexts();
}

void Title::RenderControlsTexts()
{
    float baseY = 10.0f;
    float textHeight = Renderer::GetTextHeight() + Layout::Padding * 2 + Layout::BorderWidth * 2;

    Text::Props props = { 0 };
    props.X = 10.0f;
    props.Color = Layout::TextColor;
    props.BackgroundColor = Layout::BackgroundColor;
    props.BorderWidth = Layout::BorderWidth;
    props.BorderColor = Layout::Color;
    props.BorderPosition = Border::Border_All;

    props.Y = baseY;
    props.Text = "Hold " CHAR_LT " & press " CHAR_LEFT " to " + std::string(!m_MenuOpen ? "Open." : "Close.");
    m_ControlsTexts[0].Render(props);

    props.Y = baseY + textHeight + Layout::Padding;
    props.Text = "Use " CHAR_UP CHAR_DOWN " to scroll, " CHAR_X " to select, " CHAR_RS " to go back.";
    m_ControlsTexts[1].Render(props);

    props.Y = baseY + ((textHeight + Layout::Padding) * 2);
    props.Text = "Use " CHAR_LB " & " CHAR_RB " to switch menus.";
    m_ControlsTexts[2].Render(props);
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

    pFont = R_RegisterFont("fonts/smallFont", 0);
    MaterialHandle = Material_RegisterHandle("white", 0);

    Layout::LineHeight = GetTextHeight() + Layout::Padding * 2;
}
