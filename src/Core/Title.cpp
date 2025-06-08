#include "pch.h"
#include "Core/Title.h"

#include "Core/UI.h"
#include "Modules/Binds.h"
#include "Modules/InputRecorder.h"

Title *Title::s_CurrentInstance = nullptr;
std::unordered_map<std::string, Detour> Title::s_DetourMap;

Title::Title()
    : m_InMatch(false)
{
    s_CurrentInstance = this;

    HRESULT hr = InputRecorder::Init();
    if (FAILED(hr))
        AskToReboot();
}

Title::~Title()
{
    s_CurrentInstance = nullptr;

    InputRecorder::Shutdown();
    Binds::Clear();
    RemoveHooks();
}

void Title::Update()
{
    Input::Gamepad *pGamepad = Input::GetInput();

    XASSERT(pGamepad != nullptr);

    if (m_InMatch)
    {
        // Toggle the menu by pressing LT and DPAD LEFT
        if (pGamepad->LastLeftTrigger && pGamepad->PressedButtons & XINPUT_GAMEPAD_DPAD_LEFT)
        {
            bool isOpen = m_Menu.Open();
            m_Menu.Open(!isOpen);

            return;
        }

        m_Menu.Update(pGamepad);

        if (!m_Menu.Open())
            Binds::Run(pGamepad);
    }
}

void Title::Render()
{
    if (m_InMatch)
        m_Menu.Render();
}

void Title::SCR_DrawScreenFieldHook(const int localClientNum, int refreshedUI)
{
    XASSERT(s_DetourMap.find("SCR_DrawScreenField") != s_DetourMap.end());

    // Call the original SCR_DrawScreenField function
    s_DetourMap.at("SCR_DrawScreenField").GetOriginal<decltype(&SCR_DrawScreenFieldHook)>()(localClientNum, refreshedUI);

    if (s_CurrentInstance != nullptr)
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
        hr = it->second.Install();
        if (FAILED(hr))
            break;
    }

    if (FAILED(hr))
        AskToReboot();
}

void Title::RemoveHooks()
{
    s_DetourMap.clear();
}

void Title::InitRenderer()
{
    XASSERT(UI::R_RegisterFont != nullptr);
    XASSERT(UI::Material_RegisterHandle != nullptr);

    UI::pDefaultFont = UI::R_RegisterFont("fonts/smallFont", 0);
    UI::pConsoleFont = UI::R_RegisterFont("fonts/consoleFont", 0);
    UI::pFont = UI::pDefaultFont;
    UI::MaterialHandle = UI::Material_RegisterHandle("white", 0);

    XASSERT(UI::pFont != nullptr);
    XASSERT(UI::MaterialHandle != nullptr);
}

void Title::AskToReboot()
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
