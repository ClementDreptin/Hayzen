#include "pch.h"
#include "Modules/InputRecorder.h"

namespace InputRecorder
{

static std::vector<XINPUT_GAMEPAD> s_GamepadStates;
static bool s_IsRecording = false;
static bool s_IsReplaying = false;
static Detour s_XamInputGetStateDetour;
static size_t s_ReplayIndex = 0;

static uint32_t XamInputGetStateHook(uint32_t userIndex, uint32_t flags, XINPUT_STATE *pXInputState)
{
    uint32_t result = s_XamInputGetStateDetour.GetOriginal<decltype(&XamInputGetStateHook)>()(userIndex, flags, pXInputState);

    // During the recording process, simply push the gamepad state at the current frame
    // in the global vector
    if (s_IsRecording)
    {
        XINPUT_GAMEPAD gamepad = pXInputState->Gamepad;

        // Only record some buttons to avoid recording button presses related to using
        // the menu
        uint16_t buttonsToRecord =
            XINPUT_GAMEPAD_A |
            XINPUT_GAMEPAD_B |
            XINPUT_GAMEPAD_Y |
            XINPUT_GAMEPAD_LEFT_THUMB;

        gamepad.wButtons &= buttonsToRecord;
        s_GamepadStates.emplace_back(gamepad);

        return result;
    }

    // During the replaying process, overwrite the gamepad state with what was recorded
    if (s_IsReplaying)
    {
        if (s_ReplayIndex < s_GamepadStates.size())
        {
            Memory::Write<XINPUT_GAMEPAD>(&pXInputState->Gamepad, s_GamepadStates[s_ReplayIndex]);
            s_ReplayIndex++;
        }
        else
        {
            s_IsReplaying = false;
            s_ReplayIndex = 0;
        }
    }

    return result;
}

HRESULT Init()
{
    // Preallocate a lot of states to minimize the amount of reallocations during the recording
    s_GamepadStates.reserve(1000);

    s_XamInputGetStateDetour = Detour("xam.xex", 401, XamInputGetStateHook);

    return s_XamInputGetStateDetour.Install();
}

void Shutdown()
{
    s_XamInputGetStateDetour.Remove();
    s_GamepadStates.clear();
    s_IsRecording = false;
    s_IsReplaying = false;
}

void Start()
{
    s_GamepadStates.clear();
    s_IsRecording = true;
}

void Stop()
{
    s_IsRecording = false;
}

void Replay()
{
    s_IsReplaying = true;
}

}
