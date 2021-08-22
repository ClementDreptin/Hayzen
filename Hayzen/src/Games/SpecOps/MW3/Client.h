#pragma once

#include "Games\SpecOps\MW3\Menu.h"

namespace SpecOps
{
namespace MW3
{
    class Client
    {
    public:
        Client() : m_Num(-1), m_Initialized(FALSE) {}
        Client(INT clientNum);

        Menu& GetMenu() { return m_Menu; }
        BOOL IsInitialized() CONST { return m_Initialized; }
    private:
        INT m_Num;
        Menu m_Menu;
        BOOL m_Initialized;
    };
}
}