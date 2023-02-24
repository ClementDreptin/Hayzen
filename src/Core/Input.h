#pragma once

class Input
{
public:
    struct Gamepad : public XINPUT_GAMEPAD
    {
        Gamepad()
            : XINPUT_GAMEPAD(),
              LastButtons(0), LastLeftTrigger(false), LastRightTrigger(false),
              PressedButtons(0), PressedLeftTrigger(false), PressedRightTrigger(false) {}

        // Records the state (when last updated) of the buttons.
        // These remain set as long as the button is pressed.
        uint16_t LastButtons;
        bool LastLeftTrigger;
        bool LastRightTrigger;

        // Records which buttons were pressed this frame - only set on
        // the frame that the button is first pressed.
        uint16_t PressedButtons;
        bool PressedLeftTrigger;
        bool PressedRightTrigger;
    };

    static Gamepad *GetInput();

private:
    static Gamepad s_Gamepad;

    static uint16_t ButtonForVirtualKey(uint16_t virtualKey);
};
