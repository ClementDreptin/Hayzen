//--------------------------------------------------------------------------------------
// AtgInput.h
//
// Input helper functions for samples
//
// Xbox Advanced Technology Group.
// Copyright (C) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#pragma once
#ifndef ATGINPUT_H
#define ATGINPUT_H

namespace ATG
{

    //--------------------------------------------------------------------------------------
    // Name: struct ATGGAMEPAD
    // Desc: Structure for holding Gamepad data
    //--------------------------------------------------------------------------------------
    struct GAMEPAD : public XINPUT_GAMEPAD
    {
        // The following members are inherited from XINPUT_GAMEPAD:
        //    WORD    wButtons;
        //    BYTE    bLeftTrigger;
        //    BYTE    bRightTrigger;
        //    SHORT   sThumbLX;
        //    SHORT   sThumbLY;
        //    SHORT   sThumbRX;
        //    SHORT   sThumbRY;

        // Thumb stick values converted to range [-1,+1]
        FLOAT      fX1;
        FLOAT      fY1;
        FLOAT      fX2;
        FLOAT      fY2;

        // Records the state (when last updated) of the buttons.
        // These remain set as long as the button is pressed.
        WORD       wLastButtons;
        BOOL       bLastLeftTrigger;
        BOOL       bLastRightTrigger;

        // Records which buttons were pressed this frame - only set on
        // the frame that the button is first pressed.
        WORD       wPressedButtons;
        BOOL       bPressedLeftTrigger;
        BOOL       bPressedRightTrigger;

        // Device properties
        XINPUT_CAPABILITIES caps;
        BOOL       bConnected;

        // Flags for whether game pad was just inserted or removed
        BOOL       bInserted;
        BOOL       bRemoved;

        // The user index associated with this gamepad
        DWORD      dwUserIndex;

        // Deadzone pseudo-constants for the thumbsticks
        static SHORT LEFT_THUMB_DEADZONE;
        static SHORT RIGHT_THUMB_DEADZONE;
    };


    //--------------------------------------------------------------------------------------
    // Name: class Input
    // Desc: Class to manage input devices
    //--------------------------------------------------------------------------------------
    class Input
    {
    public:
        // Global access to input states
        static GAMEPAD   m_Gamepads[4];
        static GAMEPAD   m_DefaultGamepad;

        // Processes input from the game pad
        static VOID GetInput(GAMEPAD* pGamepads = NULL);

        // Processes input from all 4 gamepads and merge it into one input
        static GAMEPAD* GetMergedInput(DWORD dwMask = 0, DWORD* pdwActiveGamePadsMask = NULL);
    };

} // namespace ATG

#endif // ATGINPUT_H
