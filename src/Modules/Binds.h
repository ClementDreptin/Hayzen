#pragma once

namespace Binds
{

typedef std::function<bool()> Callback;

void Add(uint16_t button, Callback callback);

void Remove(uint16_t button);

bool Has(uint16_t button);

void Clear();

void Run(Input::Gamepad *pGamepad);

}
