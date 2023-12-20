#include "pch.h"
#include "Core/Title.h"

#include "Core/Context.h"
#include "Core/Settings.h"
#include "UI/Renderer.h"

Title *Title::s_CurrentInstance = nullptr;
std::unordered_map<std::string, Detour *> Title::s_DetourMap;

Title::Title()
    : m_InMatch(false), m_MenuOpen(false)
{
    s_CurrentInstance = this;
}

Title::~Title()
{
    s_CurrentInstance = nullptr;

    RemoveHooks();
}

void Title::Update()
{
    Input::Gamepad *pGamepad = Input::GetInput();

    // Toggle the menu by pressing LT and DPAD LEFT
    if (pGamepad->LastLeftTrigger && pGamepad->PressedButtons & XINPUT_GAMEPAD_DPAD_LEFT)
    {
        m_MenuOpen = !m_MenuOpen;
        return;
    }

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
    if (m_MenuOpen)
        m_Menu.Render();

    if (Settings::DisplayControlsTexts)
        RenderControlsTexts();
}

void Title::RenderControlsTexts()
{
    float yOffset = 10.0f;
    float fontScale = 0.8f;
    float padding = Settings::Padding * fontScale;
    float borderWidth = Settings::BorderWidth * fontScale;

    Text::Props props = {};
    props.X = 10.0f;
    props.FontScale = fontScale;
    props.Color = Settings::TextColor;
    props.BackgroundColor = Settings::BackgroundColor;
    props.BorderWidth = borderWidth;
    props.BorderColor = Settings::Color;
    props.BorderPosition = Border::Border_All;

    props.Y = yOffset;
    props.Text = "Hold " CHAR_LT " & press " CHAR_LEFT " to " + std::string(!m_MenuOpen ? "Open." : "Close.");
    yOffset += Renderer::GetTextHeight(props.Text, fontScale) + padding * 3 + borderWidth * 2;
    m_ControlsTexts[0].Render(props);

    props.Y = yOffset;
    props.Text = "Use " CHAR_UP CHAR_DOWN " to scroll, " CHAR_X " to select, " CHAR_RS " to go back.";
    yOffset += Renderer::GetTextHeight(props.Text, fontScale) + padding * 3 + borderWidth * 2;
    m_ControlsTexts[1].Render(props);

    props.Y = yOffset;
    props.Text = "Use " CHAR_LB " & " CHAR_RB " to switch menus.";
    yOffset += Renderer::GetTextHeight(props.Text, fontScale) + padding * 3 + borderWidth * 2;
    m_ControlsTexts[2].Render(props);
}

void Title::SCR_DrawScreenFieldHook(const int localClientNum, int refreshedUI)
{
    // Call the original SCR_DrawScreenField function
    s_DetourMap.at("SCR_DrawScreenField")->GetOriginal<decltype(&SCR_DrawScreenFieldHook)>()(localClientNum, refreshedUI);

    if (s_CurrentInstance != nullptr && s_CurrentInstance->InMatch())
    {
        s_CurrentInstance->Update();

        s_CurrentInstance->Render();
    }
}

void Title::InstallHooks()
{
    HRESULT hr = S_OK;

    for (auto it = s_DetourMap.begin(); it != s_DetourMap.end(); ++it)
    {
        hr = it->second->Install();
        if (FAILED(hr))
            break;
    }

    if (FAILED(hr))
    {
        const wchar_t *buttonLabels[] = { L"Yes", L"No" };
        uint32_t buttonPressedIndex = 0;

        uint32_t result = Xam::ShowMessageBox(
            L"Error",
            L"Initialization failed. Restarting the console could fix the problem.\n\nDo you want to restart?",
            buttonLabels,
            ARRAYSIZE(buttonLabels),
            &buttonPressedIndex,
            XMB_ERRORICON,
            1
        );

        if (result == ERROR_SUCCESS && buttonPressedIndex == 0)
            Xam::Reboot();
    }
}

void Title::RemoveHooks()
{
    for (auto it = s_DetourMap.begin(); it != s_DetourMap.end(); ++it)
        delete it->second;

    s_DetourMap.clear();
}

void Title::InitRenderer()
{
    using namespace Renderer;

    pFont = R_RegisterFont("fonts/smallFont", 0);
    MaterialHandle = Material_RegisterHandle("white", 0);
}
