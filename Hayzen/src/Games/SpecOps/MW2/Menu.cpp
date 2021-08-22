#include "pch.h"
#include "Games\SpecOps\MW2\Menu.h"

#include "Games\SpecOps\MW2\Functions.h"
#include "Games\SpecOps\MW2\Events.h"

namespace SpecOps
{
namespace MW2
{
    Menu::Menu(INT clientNum)
        : m_ClientNum(clientNum), m_Open(FALSE), m_CurrentScrollerPos(0), m_SavedPos(vec3(0.0f, 0.0f, 0.0f)), m_SavedAngles(vec3(0.0f, 0.0f, 0.0f)), m_BindsEnabled(FALSE)
    {
        m_Background = RectangleElem(HudElem::s_MenuX, HudElem::s_MenuY, HudElem::s_MenuWidth, HudElem::s_MenuHeight, HudElem::s_ColorBlack);
        m_Background.SetAlpha(0.7f);

        m_Title = TextElem("Cod Jumper", HudElem::s_MenuX + HudElem::s_Padding, HudElem::s_Padding + HudElem::s_TitleHeight, HudElem::s_ColorWhite, 1.7f);

        m_Scroller = RectangleElem(HudElem::s_MenuX, HudElem::s_MenuY + (HudElem::s_Padding * 2) + HudElem::s_TitleHeight, HudElem::s_MenuWidth, HudElem::s_LineHeight, HudElem::s_ColorWhite);
        m_Scroller.SetAlpha(0.7f);

        m_Instructions = TextElem("Navigate: UP - DOWN | Select: X | Back: RS",
            HudElem::s_MenuX + HudElem::s_Padding, HudElem::s_MenuY + HudElem::s_MenuHeight - HudElem::s_Padding - 80, HudElem::s_ColorWhite, 0.7f);

        CreateStructure();
    }

    VOID Menu::ToggleGodMode()
    {
        playerState_s* playerState = SV_GetPlayerstateForClientNum(m_ClientNum);

        if (playerState->otherFlags == 0)
        {
            playerState->otherFlags = 1;
            iPrintLn(m_ClientNum, "God Mode ^2On");
        }
        else
        {
            playerState->otherFlags = 0;
            iPrintLn(m_ClientNum, "God Mode ^1Off");
        }
    }

    VOID Menu::ToggleAmmo()
    {
        DWORD address = 0x82331F48;
        UINT defaultValue = 0x7D1D4850;
        UINT modifiedValue = 0x7D284B78;

        if (Memory::Read<UINT>(address) == defaultValue)
        {
            Memory::Write<UINT>(address, modifiedValue);
            iPrintLn(m_ClientNum, "Unlimited Ammo ^2On");
        }
        else
        {
            Memory::Write<UINT>(address, defaultValue);
            iPrintLn(m_ClientNum, "Unlimited Ammo ^1Off");
        }
    }

    VOID Menu::ToggleUFO()
    {
        if (GetGClient(m_ClientNum)->mFlags != 2)
        {
            GetGClient(m_ClientNum)->mFlags = 2;
            iPrintLn(m_ClientNum, "Ufo ^2On");
        }
        else
        {
            GetGClient(m_ClientNum)->mFlags = 0;
            iPrintLn(m_ClientNum, "Ufo ^1Off");
        }
    }

    VOID Menu::SavePosition()
    {
        m_SavedPos = SV_GetPlayerstateForClientNum(m_ClientNum)->origin;
        m_SavedAngles = SV_GetPlayerstateForClientNum(m_ClientNum)->viewAngles;

        iPrintLn(m_ClientNum, "Position ^2Saved");
    }

    VOID Menu::LoadPosition()
    {
        if (m_SavedPos == vec3(0.0f, 0.0f, 0.0f) || m_SavedAngles == vec3(0.0f, 0.0f, 0.0f))
        {
            iPrintLn(m_ClientNum, "^1Save a position first!");
            return;
        }

        FLOAT origin[] = { m_SavedPos.x, m_SavedPos.y, m_SavedPos.z };
        FLOAT angles[] = { m_SavedAngles.x, m_SavedAngles.y, m_SavedAngles.z };

        TeleportPlayer(GetEntity(m_ClientNum), origin, angles);
    }

    VOID Menu::ToggleSaveLoadBinds()
    {
        if (!m_BindsEnabled)
            iPrintLn(m_ClientNum, "Press RB to ^2Save^7 and LB to ^2Load");
        else
            iPrintLn(m_ClientNum, "Save and Load binds ^1Off");

        m_BindsEnabled = !m_BindsEnabled;
    }

    VOID Menu::CreateStructure()
    {
        m_Structure["Cod Jumper"] = std::vector<Option>();
        m_Structure["Cod Jumper"].reserve(2);
        m_Structure["Cod Jumper"].emplace_back(Option("Main", 0));
        m_Structure["Cod Jumper"].emplace_back(Option("Teleport", 1));

        m_Structure["Main"] = std::vector<Option>();
        m_Structure["Main"].reserve(2);
        m_Structure["Main"].emplace_back(Option("God Mode", 0));
        m_Structure["Main"].emplace_back(Option("Ammo", 1));

        m_Structure["Teleport"] = std::vector<Option>();
        m_Structure["Teleport"].reserve(4);
        m_Structure["Teleport"].emplace_back(Option("Save/Load Binds", 0));
        m_Structure["Teleport"].emplace_back(Option("Save Position", 1));
        m_Structure["Teleport"].emplace_back(Option("Load Position", 2));
        m_Structure["Teleport"].emplace_back(Option("UFO", 3));
    }

    VOID Menu::OnSelectPressed(CONST std::string& optionName)
    {
        if (optionName == "Main" || optionName == "Teleport")
            GoToMenu(optionName);
        else if (optionName == "God Mode")
            ToggleGodMode();
        else if (optionName == "Ammo")
            ToggleAmmo();
        else if (optionName == "UFO")
            ToggleUFO();
        else if (optionName == "Save Position")
            SavePosition();
        else if (optionName == "Load Position")
            LoadPosition();
        else if (optionName == "Save/Load Binds")
            ToggleSaveLoadBinds();
    }

    VOID Menu::OnBackPressed(CONST std::string& optionName)
    {
        if (optionName == "Main" || optionName == "Teleport")
            return;

        GoToMenu("Cod Jumper");
    }

    VOID Menu::GoToMenu(CONST std::string& menuName)
    {
        ResetScroller();

        m_Title.SetText(menuName);
    }

    VOID Menu::Update()
    {
        if (!m_Open)
            return;

        m_Background.Draw();
        m_Title.Draw();
        m_Scroller.Draw();
        m_Instructions.Draw();
        
        for (size_t i = 0; i < m_Structure[m_Title.GetText()].size(); i++)
            m_Structure[m_Title.GetText()][i].Draw();

    }

    VOID Menu::OnEvent(CONST std::string& eventString)
    {
        if (eventString == DPAD_LEFT)
            m_Open = !m_Open;

        if (eventString == DPAD_UP && m_Open)
        {
            m_CurrentScrollerPos--;

            if (m_CurrentScrollerPos < 0)
                m_CurrentScrollerPos = m_Structure[m_Title.GetText()].size() - 1;

            MoveScroller(m_CurrentScrollerPos);
        }

        if (eventString == DPAD_DOWN && m_Open)
        {
            m_CurrentScrollerPos++;

            if (m_CurrentScrollerPos >= (INT)m_Structure[m_Title.GetText()].size())
                m_CurrentScrollerPos = 0;

            MoveScroller(m_CurrentScrollerPos);
        }

        if (eventString == SELECT && m_Open)
            OnSelectPressed(m_Structure[m_Title.GetText()][m_CurrentScrollerPos].GetText());
        
        if (eventString == BACK && m_Open)
            OnBackPressed(m_Structure[m_Title.GetText()][m_CurrentScrollerPos].GetText());

        if (eventString == LB && m_BindsEnabled)
            LoadPosition();

        if (eventString == RB && m_BindsEnabled)
            SavePosition();
    }

    VOID Menu::MoveScroller(INT position)
    {
        m_Scroller.SetY(HudElem::s_MenuY + HudElem::s_Padding * 2 + HudElem::s_TitleHeight + HudElem::s_LineHeight * position);
    }

    VOID Menu::ResetScroller()
    {
        m_CurrentScrollerPos = 0;
        MoveScroller(m_CurrentScrollerPos);
    }
}
}