#include "pch.h"
#include "Modules/Binds.h"

namespace Binds
{

static std::unordered_map<uint16_t, Callback> s_Map;

void Add(uint16_t button, Callback callback)
{
    s_Map[button] = callback;
}

void Remove(uint16_t button)
{
    s_Map.erase(button);
}

bool Has(uint16_t button)
{
    return s_Map.find(button) != s_Map.end();
}

void Clear()
{
    s_Map.clear();
}

void Run(Input::Gamepad *pGamepad)
{
    for (auto it = s_Map.begin(); it != s_Map.end(); ++it)
    {
        const uint16_t &button = it->first;
        const Callback &callback = it->second;

        if (pGamepad->PressedButtons & button)
        {
            callback();
            break;
        }
    }
}

}
