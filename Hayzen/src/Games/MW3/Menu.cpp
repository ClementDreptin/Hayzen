#include "pch.h"
#include "Games\MW3\Menu.h"

#include "Games\MW3\Functions.h"
#include "Games\MW3\MW3.h"
#include "Games\MW3\Events.h"

namespace MW3
{
    Menu::Menu(int clientNum)
        : m_ClientNum(clientNum), m_Open(false), m_CurrentScrollerPos(0), m_SavedPos(vec3(0.0f, 0.0f, 0.0f)), m_SavedAngles(vec3(0.0f, 0.0f, 0.0f)), m_BindsEnabled(false)
    {
        m_Background = RectangleElem(clientNum, HudElem::s_MenuX, HudElem::s_MenuY, HudElem::s_MenuWidth, HudElem::s_MenuHeight, HudElem::s_ColorBlackNoAlpha);

        m_Title = TextElem(clientNum, "Cod Jumper", HudElem::s_MenuX + HudElem::s_Padding, HudElem::s_MenuY + HudElem::s_Padding + HudElem::s_TitleHeight, HudElem::s_ColorWhiteNoAlpha, 3.0f);

        m_Scroller = RectangleElem(clientNum, HudElem::s_MenuX, HudElem::s_MenuY + (HudElem::s_Padding * 2) + HudElem::s_TitleHeight, HudElem::s_MenuWidth, (int)HudElem::s_LineHeight, HudElem::s_ColorWhiteNoAlpha);

        m_Instructions = TextElem(clientNum, "Navigate: [{+actionslot 1}] - [{+actionslot 2}] | Select: [{+usereload}] | Back: [{+melee}]",
            HudElem::s_MenuX + HudElem::s_Padding, HudElem::s_MenuY + HudElem::s_MenuHeight - HudElem::s_Padding - 80, HudElem::s_ColorWhiteNoAlpha, 1.5f);

        CreateStructure();
    }

    void Menu::ToggleDepatchBounces()
    {
        DWORD branchAddress = 0x820EB474;
        unsigned int defaultValue = 0x409AFFB0;
        unsigned int modifiedValue = 0x6060FFB0;

        if (Memory::Read<unsigned int>(branchAddress) == defaultValue)
        {
            Memory::Write<unsigned int>(branchAddress, modifiedValue);
            iPrintLn(m_ClientNum, "Depatch Bounces ^2On");
        }
        else
        {
            Memory::Write<unsigned int>(branchAddress, defaultValue);
            iPrintLn(m_ClientNum, "Depatch Bounces ^1Off");
        }
    }

    void Menu::ToggleFallDamage()
    {
        DWORD address = 0x82000C04;

        if (Memory::Read<float>(address) == 128.0f)
        {
            Memory::Write<float>(address, 9999.0f);
            iPrintLn(m_ClientNum, "Fall Damage ^2Off");
        }
        else
        {
            Memory::Write<float>(address, 128.0f);
            iPrintLn(m_ClientNum, "Fall Damage ^1On");
        }
    }

    void Menu::ToggleAmmo()
    {
        DWORD address = 0x820F63E4;
        unsigned int defaultValue = 0x7D3D5050;
        unsigned int modifiedValue = 0x7D495378;

        if (Memory::Read<unsigned int>(address) == defaultValue)
        {
            Memory::Write<unsigned int>(address, modifiedValue);
            iPrintLn(m_ClientNum, "Unlimited Ammo ^2On");
        }
        else
        {
            Memory::Write<unsigned int>(address, defaultValue);
            iPrintLn(m_ClientNum, "Unlimited Ammo ^1Off");
        }
    }

    void Menu::ToggleUFO()
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

    void Menu::SavePosition()
    {
        m_SavedPos = GetPlayerState(m_ClientNum)->origin;
        m_SavedAngles = GetPlayerState(m_ClientNum)->viewAngles;

        iPrintLn(m_ClientNum, "Position ^2Saved");
    }

    void Menu::LoadPosition()
    {
        if (m_SavedPos == vec3(0.0f, 0.0f, 0.0f) || m_SavedAngles == vec3(0.0f, 0.0f, 0.0f))
        {
            iPrintLn(m_ClientNum, "^1Save a position first!");
            return;
        }

        float origin[] = { m_SavedPos.x, m_SavedPos.y, m_SavedPos.z };
        float angles[] = { m_SavedAngles.x, m_SavedAngles.y, m_SavedAngles.z };

        TeleportPlayer(GetEntity(m_ClientNum), origin, angles);
    }

    void Menu::ToggleSaveLoadBinds()
    {
        if (!m_BindsEnabled)
            iPrintLn(m_ClientNum, "Press [{+frag}] to ^2Save^7 and [{+smoke}] to ^2Load");
        else
            iPrintLn(m_ClientNum, "Save and Load binds ^1Off");

        m_BindsEnabled = !m_BindsEnabled;
    }

    void Menu::ToggleGodMode()
    {
        int GOD_MODE_ON = 4097;
        int GOD_MODE_OFF = 4096;

        if (GetEntity(m_ClientNum)->flags == GOD_MODE_OFF)
        {
            GetEntity(m_ClientNum)->flags = GOD_MODE_ON;
            iPrintLn(m_ClientNum, "God Mode ^2On");
        }
        else
        {
            GetEntity(m_ClientNum)->flags = GOD_MODE_OFF;
            iPrintLn(m_ClientNum, "God Mode ^1Off");
        }
    }

    void Menu::Verify(int clientNum)
    {
        if (MW3::Verify(clientNum))
            iPrintLn(clientNum, "You have been ^2Verified^7, press [{+actionslot 3}] to ^2Open");
        else
            iPrintLn(m_ClientNum, "^1This player is already Verified!");
    }

    void Menu::SpawnCP()
    {
        gentity_s* currentMapBrushModel = GetCurrentMapBrushModel();
        if (!currentMapBrushModel)
        {
            iPrintLn(m_ClientNum, "^1You cannot spawn a Care Package on this map!");
            return;
        }

        float distance = 150.0f;
        vec3 origin = GetPlayerState(m_ClientNum)->origin;
        float viewY = GetPlayerState(m_ClientNum)->viewAngles.y;

        gentity_s* entity = G_Spawn();
        entity->r.currentOrigin = Math::ToFront(origin, viewY, distance);
        entity->r.currentAngles.y = viewY;

        G_SetModel(entity, "com_plasticcase_friendly");
        SP_script_model(entity);
        SV_UnlinkEntity(entity);
        entity->r.bmodel = 4;
        entity->state.index = currentMapBrushModel->state.index;
        
        int contents = entity->r.contents;
        SV_SetBrushModel(entity);
        contents |= entity->r.contents;
        entity->r.contents = contents;

        SV_LinkEntity(entity);
    }

    void Menu::CreateStructure()
    {
        m_Structure["Cod Jumper"] = std::vector<Option>();
        m_Structure["Cod Jumper"].reserve(3);
        m_Structure["Cod Jumper"].emplace_back(Option(m_ClientNum, "Main", 0));
        m_Structure["Cod Jumper"].emplace_back(Option(m_ClientNum, "Teleport", 1));
        m_Structure["Cod Jumper"].emplace_back(Option(m_ClientNum, "Admin", 2));

        m_Structure["Main"] = std::vector<Option>();
        m_Structure["Main"].reserve(5);
        m_Structure["Main"].emplace_back(Option(m_ClientNum, "God Mode", 0));
        m_Structure["Main"].emplace_back(Option(m_ClientNum, "Fall Damage", 1));
        m_Structure["Main"].emplace_back(Option(m_ClientNum, "Ammo", 2));
        m_Structure["Main"].emplace_back(Option(m_ClientNum, "Depatch Bounces", 3));
        m_Structure["Main"].emplace_back(Option(m_ClientNum, "Spawn Care Package", 4));

        m_Structure["Teleport"] = std::vector<Option>();
        m_Structure["Teleport"].reserve(4);
        m_Structure["Teleport"].emplace_back(Option(m_ClientNum, "Save/Load Binds", 0));
        m_Structure["Teleport"].emplace_back(Option(m_ClientNum, "Save Position", 1));
        m_Structure["Teleport"].emplace_back(Option(m_ClientNum, "Load Position", 2));
        m_Structure["Teleport"].emplace_back(Option(m_ClientNum, "UFO", 3));

        m_Structure["Admin"] = std::vector<Option>();
        m_Structure["Admin"].reserve(1);
        m_Structure["Admin"].emplace_back(Option(m_ClientNum, "Verify", 0));
            m_Structure["Verify"] = std::vector<Option>();
    }

    void Menu::OnSelectPressed(const std::string& optionName)
    {
        int pos;

        if (optionName == "Main" || optionName == "Teleport" || optionName == "Admin" || optionName == "Infect")
            GoToMenu(optionName);
        else if (optionName == "Depatch Bounces")
            ToggleDepatchBounces();
        else if (optionName == "Fall Damage")
            ToggleFallDamage();
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
        else if (optionName == "God Mode")
            ToggleGodMode();
        else if (optionName == "Spawn Care Package")
            SpawnCP();
        else if (optionName == "Verify")
        {
            GetAllPlayers();
            GoToMenu(optionName);
        }
        else if ((pos = optionName.find("(")) != std::string::npos)
            Verify(std::stoi(optionName.substr(pos + 1, 1)));
    }

    void Menu::OnBackPressed(const std::string& optionName)
    {
        if (optionName == "Main" || optionName == "Teleport" || optionName == "Admin")
            return;

        if (optionName == "Knockback" || optionName.find("(") != std::string::npos)
        {
            GoToMenu("Admin");
            return;
        }

        GoToMenu("Cod Jumper");
    }

    void Menu::Open()
    {
        m_Background.SetAlpha(180);
        m_Title.SetAlpha(255);
        m_Scroller.SetAlpha(180);
        m_Instructions.SetAlpha(255);

        GoToMenu(m_Title.GetText());

        m_Open = true;
    }

    void Menu::Close()
    {
        m_Background.SetAlpha(0);
        m_Title.SetAlpha(0);
        m_Scroller.SetAlpha(0);
        m_Instructions.SetAlpha(0);

        for (auto it = m_Structure.begin(); it != m_Structure.end(); it++)
            for (size_t i = 0; i < it->second.size(); i++)
                it->second[i].SetAlpha(0);

        m_Open = false;
    }

    void Menu::GoToMenu(const std::string& menuName)
    {
        ResetScroller();

        m_Title.SetText(menuName);

        for (auto it = m_Structure.begin(); it != m_Structure.end(); it++)
            for (size_t i = 0; i < it->second.size(); i++)
                it->second[i].SetAlpha(0);

        for (size_t i = 0; i < m_Structure[menuName].size(); i++)
            m_Structure[menuName][i].SetAlpha(255);
    }

    void Menu::GetAllPlayers()
    {
        m_Structure["Verify"].clear();

        int playerCount = 0;

        for (int i = 0; i < 18; i++)
        {
            if (!strcmp(GetClientState(i)->name, ""))
                continue;

            std::string optionName = std::string(GetClientState(i)->name) + " (" + std::to_string((long long)i) + ")";
            m_Structure["Verify"].emplace_back(Option(m_ClientNum, optionName, playerCount));
            playerCount++;
        }
    }

    void Menu::OnEvent(const std::string& eventString)
    {
        if (eventString == DPAD_LEFT)
        {
            if (!m_Open)
                Open();
            else
                Close();
        }

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

            if (m_CurrentScrollerPos >= (int)m_Structure[m_Title.GetText()].size())
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

    void Menu::MoveScroller(int position)
    {
        m_Scroller.SetY(HudElem::s_MenuY + HudElem::s_Padding * 2 + HudElem::s_TitleHeight + HudElem::s_LineHeight * position);
    }

    void Menu::ResetScroller()
    {
        m_CurrentScrollerPos = 0;
        MoveScroller(m_CurrentScrollerPos);
    }
}