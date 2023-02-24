#include "pch.h"
#include "Core/Callbacks.h"

#include "Core/Input.h"

namespace Callback
{

void ClickCallback(void *)
{
    Log::Info("Clicked");
}

void RangeCallback(void *pParameters)
{
    float value = *reinterpret_cast<float *>(pParameters);

    Log::Info("number: %f", value);
}

void ToggleCallback(void *pParameters)
{
    bool value = *reinterpret_cast<bool *>(pParameters);

    Log::Info(value ? "On" : "Off");
}

static bool bindsEnabled = false;

static uint32_t BindsThread(void *)
{
    while (bindsEnabled)
    {
        Input::Gamepad *pGamepad = Input::GetInput();

        Sleep(50);

        if (pGamepad->PressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
            Log::Info("Loaded");
        else if (pGamepad->PressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
            Log::Info("Saved");
    }

    return 0;
}

void ToggleBinds(void *pParameters)
{
    bindsEnabled = *reinterpret_cast<bool *>(pParameters);

    if (bindsEnabled)
        CreateThread(nullptr, 0, reinterpret_cast<PTHREAD_START_ROUTINE>(BindsThread), nullptr, 0, nullptr);
}

}
