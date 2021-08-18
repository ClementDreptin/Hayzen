#include "pch.h"
#include "Games\MW2\Client.h"

namespace MW2
{
    Client::Client(int clientNum)
        : m_Num(clientNum)
    {
        m_Menu = Menu(clientNum);
        m_Initialized = true;
    }
}