#include "pch.h"
#include "Games\SpecOps\MW3\Client.h"

namespace SpecOps
{
namespace MW3
{
    Client::Client(int clientNum)
        : m_Num(clientNum)
    {
        m_Menu = Menu(clientNum);
        m_Initialized = true;
    }
}
}