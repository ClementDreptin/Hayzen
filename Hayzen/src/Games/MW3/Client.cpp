#include "pch.h"
#include "Games\MW3\Client.h"

namespace MW3
{
    Client::Client(INT clientNum)
        : m_Num(clientNum)
    {
        m_Menu = Menu(clientNum);
        m_Initialized = TRUE;
    }
}