#include "pch.h"
#include "Games\SpecOps\MW2\Client.h"

namespace SpecOps
{
namespace MW2
{
    Client::Client(int clientNum)
        : m_Num(clientNum)
    {
        m_Menu = Menu(clientNum);
        m_Initialized = true;
    }
}
}