#include "pch.h"
#include "Core/Input.h"

Input::Gamepad Input::s_Gamepad;

Input::Gamepad *Input::GetInput()
{
    // Get the gamepad state
    XINPUT_STATE state = { 0 };
    DWORD result = XInputGetState(0, &state);

    // If the first controller is not connected, just return early
    if (result != ERROR_SUCCESS)
        return &s_Gamepad;

    // Copy the gamepad to the local structure
    memcpy_s(&s_Gamepad, sizeof(Input::Gamepad), &state.Gamepad, sizeof(XINPUT_GAMEPAD));

    // Save the buttons pressed at the previous frame to set the currently pressed buttons only if
    // they were not already pressed at the previous frame, we need to do this because pressing
    // then releasing a button (even done really fast) takes multiple frames.
    s_Gamepad.PressedButtons = (s_Gamepad.LastButtons ^ s_Gamepad.wButtons) & s_Gamepad.wButtons;
    s_Gamepad.LastButtons = s_Gamepad.wButtons;

    // Get the keystrokes
    XINPUT_KEYSTROKE keystroke = { 0 };
    result = XInputGetKeystroke(0, XINPUT_FLAG_GAMEPAD, &keystroke);

    // If no button is pressed, just return early
    if (result != ERROR_SUCCESS)
        return &s_Gamepad;

    // If a key is being held, add it to the pressed buttons even if it was already pressed at the previous frame
    if (keystroke.Flags & XINPUT_KEYSTROKE_REPEAT)
        s_Gamepad.PressedButtons |= ButtonForVirtualKey(keystroke.VirtualKey);

    // Check if the left trigger is pressed
    bool leftTriggerPressed = (s_Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
    if (leftTriggerPressed)
        s_Gamepad.PressedLeftTrigger = !s_Gamepad.LastLeftTrigger;
    else
        s_Gamepad.PressedLeftTrigger = false;

    // Store the left trigger state for next time
    s_Gamepad.LastLeftTrigger = leftTriggerPressed;

    // Check if the left trigger is pressed
    bool rightTriggerPressed = (s_Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
    if (rightTriggerPressed)
        s_Gamepad.PressedRightTrigger = !s_Gamepad.LastRightTrigger;
    else
        s_Gamepad.PressedRightTrigger = false;

    // Store the left trigger state for next time
    s_Gamepad.LastRightTrigger = leftTriggerPressed;

    return &s_Gamepad;
}

uint16_t Input::ButtonForVirtualKey(uint16_t virtualKey)
{
    switch (virtualKey)
    {
    case VK_PAD_A:
        return XINPUT_GAMEPAD_A;
    case VK_PAD_B:
        return XINPUT_GAMEPAD_B;
    case VK_PAD_X:
        return XINPUT_GAMEPAD_X;
    case VK_PAD_Y:
        return XINPUT_GAMEPAD_Y;
    case VK_PAD_RSHOULDER:
        return XINPUT_GAMEPAD_RIGHT_SHOULDER;
    case VK_PAD_LSHOULDER:
        return XINPUT_GAMEPAD_LEFT_SHOULDER;
    case VK_PAD_DPAD_UP:
        return XINPUT_GAMEPAD_DPAD_UP;
    case VK_PAD_DPAD_DOWN:
        return XINPUT_GAMEPAD_DPAD_DOWN;
    case VK_PAD_DPAD_LEFT:
        return XINPUT_GAMEPAD_DPAD_LEFT;
    case VK_PAD_DPAD_RIGHT:
        return XINPUT_GAMEPAD_DPAD_RIGHT;
    case VK_PAD_START:
        return XINPUT_GAMEPAD_START;
    case VK_PAD_BACK:
        return XINPUT_GAMEPAD_BACK;
    case VK_PAD_LTHUMB_PRESS:
        return XINPUT_GAMEPAD_LEFT_THUMB;
    case VK_PAD_RTHUMB_PRESS:
        return XINPUT_GAMEPAD_RIGHT_THUMB;
    default:
        return 0;
    }
}
