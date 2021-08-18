#pragma once

#include "Games\MW3\Menu.h"

namespace MW3
{
    class Client
    {
    public:
        Client() : m_Num(-1), m_Initialized(false) {}
        Client(int clientNum);

        Menu& GetMenu() { return m_Menu; }
        bool IsInitialized() const { return m_Initialized; }
    private:
        int m_Num;
        Menu m_Menu;
        bool m_Initialized;
    };
}