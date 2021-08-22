#include "pch.h"
#include "Games\SpecOps\MW2\Client.h"

namespace SpecOps
{
namespace MW2
{
    Client::Client(INT clientNum)
        : m_Num(clientNum)
    {
        m_Menu = Menu(clientNum);
        m_Initialized = TRUE;
    }
}
}