//--------------------------------------------------------------------------------------
// AtgInput.cpp
//
// Input helper functions for samples
//
// Xbox Advanced Technology Group.
// Copyright (C) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "AtgInput.h"

namespace ATG
{

    //--------------------------------------------------------------------------------------
    // Globals
    //--------------------------------------------------------------------------------------

    // Instance of static member variables
    GAMEPAD   Input::m_Gamepads[4];
    GAMEPAD   Input::m_DefaultGamepad;

    // Deadzone pseudo-constants for the thumbsticks
    SHORT GAMEPAD::LEFT_THUMB_DEADZONE = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
    SHORT GAMEPAD::RIGHT_THUMB_DEADZONE = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;


    //--------------------------------------------------------------------------------------
    // Name: ConvertThumbstickValue()
    // Desc: Converts SHORT thumbstick values to FLOAT, while enforcing a deadzone
    //--------------------------------------------------------------------------------------
    inline FLOAT ConvertThumbstickValue(SHORT sThumbstickValue, SHORT sDeadZone)
    {
        if (sThumbstickValue > +sDeadZone)
        {
            return (sThumbstickValue - sDeadZone) / (32767.0f - sDeadZone);
        }
        if (sThumbstickValue < -sDeadZone)
        {
            return (sThumbstickValue + sDeadZone + 1.0f) / (32767.0f - sDeadZone);
        }
        return 0.0f;
    }


    //--------------------------------------------------------------------------------------
    // Name: GetInput()
    // Desc: Processes input from the gamepads
    //--------------------------------------------------------------------------------------
    VOID Input::GetInput(GAMEPAD* pGamepads)
    {
        // Initial the static data structures
        static BOOL bInitialized = FALSE;
        if (FALSE == bInitialized)
        {
            ZeroMemory(m_Gamepads, sizeof(m_Gamepads));
            ZeroMemory(&m_DefaultGamepad, sizeof(m_DefaultGamepad));
            bInitialized = TRUE;
        }

        // If the user did not specify a list of gamepads, use the global list
        if (NULL == pGamepads)
            pGamepads = m_Gamepads;

        // Loop through all gamepads
        for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
        {
            // Read the input state
            XINPUT_STATE InputState;
            BOOL bWasConnected = pGamepads[i].bConnected;
            pGamepads[i].bConnected = (XInputGetState(i, &InputState) == ERROR_SUCCESS) ? TRUE : FALSE;

            // Track insertion and removals
            pGamepads[i].bRemoved = (bWasConnected && !pGamepads[i].bConnected) ? TRUE : FALSE;
            pGamepads[i].bInserted = (!bWasConnected && pGamepads[i].bConnected) ? TRUE : FALSE;

            if (FALSE == pGamepads[i].bConnected)
                continue;

            // Store the capabilities of the device
            if (TRUE == pGamepads[i].bInserted)
            {
                ZeroMemory(&pGamepads[i], sizeof(m_Gamepads[i]));
                pGamepads[i].bConnected = TRUE;
                pGamepads[i].bInserted = TRUE;
                XInputGetCapabilities(i, XINPUT_FLAG_GAMEPAD, &pGamepads[i].caps);
            }

            // Copy gamepad to local structure
            memcpy(&pGamepads[i], &InputState.Gamepad, sizeof(XINPUT_GAMEPAD));

            // Put Xbox device input for the gamepad into our custom format
            pGamepads[i].fX1 = ConvertThumbstickValue(pGamepads[i].sThumbLX, GAMEPAD::LEFT_THUMB_DEADZONE);
            pGamepads[i].fY1 = ConvertThumbstickValue(pGamepads[i].sThumbLY, GAMEPAD::LEFT_THUMB_DEADZONE);
            pGamepads[i].fX2 = ConvertThumbstickValue(pGamepads[i].sThumbRX, GAMEPAD::RIGHT_THUMB_DEADZONE);
            pGamepads[i].fY2 = ConvertThumbstickValue(pGamepads[i].sThumbRY, GAMEPAD::RIGHT_THUMB_DEADZONE);

            // Get the boolean buttons that have been pressed since the last
            // call. Each button is represented by one bit.
            pGamepads[i].wPressedButtons = (pGamepads[i].wLastButtons ^ pGamepads[i].wButtons) & pGamepads[i].wButtons;
            pGamepads[i].wLastButtons = pGamepads[i].wButtons;

            // Figure out if the left trigger has been pressed or released
            BOOL bPressed = (pGamepads[i].bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD);

            if (bPressed)
                pGamepads[i].bPressedLeftTrigger = !pGamepads[i].bLastLeftTrigger;
            else
                pGamepads[i].bPressedLeftTrigger = FALSE;

            // Store the state for next time
            pGamepads[i].bLastLeftTrigger = bPressed;

            // Figure out if the right trigger has been pressed or released
            bPressed = (pGamepads[i].bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD);

            if (bPressed)
                pGamepads[i].bPressedRightTrigger = !pGamepads[i].bLastRightTrigger;
            else
                pGamepads[i].bPressedRightTrigger = FALSE;

            // Store the state for next time
            pGamepads[i].bLastRightTrigger = bPressed;

            // Set the user index for this gamepad
            pGamepads[i].dwUserIndex = i;
        }
    }


    //--------------------------------------------------------------------------------------
    // Name: ClampToShort()
    // Desc: Takes an integer input and clamps it to fit in a short, to avoid wrapping.
    //--------------------------------------------------------------------------------------
    SHORT ClampToShort(INT input)
    {
        // Use min/max from windef.h. Define NOMINMAX if you don't want these
        // macros defined.
        input = min(input, SHRT_MAX);
        input = max(input, SHRT_MIN);
        return (SHORT)input;
    }


    //--------------------------------------------------------------------------------------
    // Name: GetMergedInput()
    // Desc: Processes input from all gamepads and merge it into one input. This is done for
    //       expediency in samples and is not typically useful or advised for games.
    //       If the pdwActiveGamePadsMask is non-NULL, then returns active gamepads.
    //       
    //--------------------------------------------------------------------------------------
    GAMEPAD* Input::GetMergedInput(DWORD dwMask, DWORD* pdwActiveGamePadsMask)
    {
        // Get input for ALL the gamepads
        GetInput();

        // Sum input across ALL gamepads into one default structure.
        ZeroMemory(&m_DefaultGamepad, sizeof(m_DefaultGamepad));
        INT  iThumbLX = 0;
        INT  iThumbLY = 0;
        INT  iThumbRX = 0;
        INT  iThumbRY = 0;
        BOOL bActiveThumbs[XUSER_MAX_COUNT] = { 0 };
        BOOL bActiveButtons[XUSER_MAX_COUNT] = { 0 };

        for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
        {
            if (m_Gamepads[i].bConnected && (!dwMask || (dwMask & (1 << i))))
            {
                SHORT sThumbLX = 0;
                SHORT sThumbLY = 0;
                SHORT sThumbRX = 0;
                SHORT sThumbRY = 0;

                // Only account for thumbstick info beyond the deadzone
                // Note that this is a simplification of the deadzone. The dead
                // zone is when the thumb stick is at or near its 'home' position.
                // If the thumb stick is pushed all the way to the left then, even
                // though the y-coordinates are still in the dead-zone range, they
                // are actually considered significant. This dead-zone ignoring
                // code will ignore the y-coordinate in a horizontal block all the
                // way across, and will ignore the x-coordinate in a vertical block
                // all the way up/down. This simplification lets us normalize the
                // coordinates into a simple +-1.0 range, but for delicate control
                // (subtle steering when pushing forward) this technique will not
                // be sufficient.
                if (m_Gamepads[i].sThumbLX > GAMEPAD::LEFT_THUMB_DEADZONE ||
                    m_Gamepads[i].sThumbLX < -GAMEPAD::LEFT_THUMB_DEADZONE)
                    sThumbLX = m_Gamepads[i].sThumbLX;
                if (m_Gamepads[i].sThumbLY > GAMEPAD::LEFT_THUMB_DEADZONE ||
                    m_Gamepads[i].sThumbLY < -GAMEPAD::LEFT_THUMB_DEADZONE)
                    sThumbLY = m_Gamepads[i].sThumbLY;
                if (m_Gamepads[i].sThumbRX > GAMEPAD::RIGHT_THUMB_DEADZONE ||
                    m_Gamepads[i].sThumbRX < -GAMEPAD::RIGHT_THUMB_DEADZONE)
                    sThumbRX = m_Gamepads[i].sThumbRX;
                if (m_Gamepads[i].sThumbRY > GAMEPAD::RIGHT_THUMB_DEADZONE ||
                    m_Gamepads[i].sThumbRY < -GAMEPAD::RIGHT_THUMB_DEADZONE)
                    sThumbRY = m_Gamepads[i].sThumbRY;

                // Sum up the raw thumbstick inputs, as long as the thumbstick
                // is outside of the dead-zone. This is different from the
                // summing above where the x and y values are accepted or
                // discarded separately.
                if (m_Gamepads[i].sThumbRX > GAMEPAD::RIGHT_THUMB_DEADZONE ||
                    m_Gamepads[i].sThumbRX < -GAMEPAD::RIGHT_THUMB_DEADZONE ||
                    m_Gamepads[i].sThumbRY > GAMEPAD::RIGHT_THUMB_DEADZONE ||
                    m_Gamepads[i].sThumbRY < -GAMEPAD::RIGHT_THUMB_DEADZONE)
                {
                    iThumbRX += m_Gamepads[i].sThumbRX;
                    iThumbRY += m_Gamepads[i].sThumbRY;
                }

                if (m_Gamepads[i].sThumbLX > GAMEPAD::LEFT_THUMB_DEADZONE ||
                    m_Gamepads[i].sThumbLX < -GAMEPAD::LEFT_THUMB_DEADZONE ||
                    m_Gamepads[i].sThumbLY > GAMEPAD::LEFT_THUMB_DEADZONE ||
                    m_Gamepads[i].sThumbLY < -GAMEPAD::LEFT_THUMB_DEADZONE)
                {
                    iThumbLX += m_Gamepads[i].sThumbLX;
                    iThumbLY += m_Gamepads[i].sThumbLY;
                }

                // Keep track of which gamepads are active
                if (sThumbLX != 0 || sThumbLY != 0 || sThumbRX != 0 || sThumbRY != 0)
                    bActiveThumbs[i] = TRUE;

                m_DefaultGamepad.fX1 += m_Gamepads[i].fX1;
                m_DefaultGamepad.fY1 += m_Gamepads[i].fY1;
                m_DefaultGamepad.fX2 += m_Gamepads[i].fX2;
                m_DefaultGamepad.fY2 += m_Gamepads[i].fY2;

                m_DefaultGamepad.wButtons |= m_Gamepads[i].wButtons;
                m_DefaultGamepad.wPressedButtons |= m_Gamepads[i].wPressedButtons;
                m_DefaultGamepad.wLastButtons |= m_Gamepads[i].wLastButtons;

                if (m_Gamepads[i].wButtons != m_Gamepads[i].wLastButtons)
                    bActiveButtons[i] = TRUE;

                for (DWORD b = 0; b < 8; b++)
                {
                    m_DefaultGamepad.bLeftTrigger |= m_Gamepads[i].bLeftTrigger;
                    m_DefaultGamepad.bPressedLeftTrigger |= m_Gamepads[i].bPressedLeftTrigger;
                    m_DefaultGamepad.bLastLeftTrigger |= m_Gamepads[i].bLastLeftTrigger;

                    m_DefaultGamepad.bRightTrigger |= m_Gamepads[i].bRightTrigger;
                    m_DefaultGamepad.bPressedRightTrigger |= m_Gamepads[i].bPressedRightTrigger;
                    m_DefaultGamepad.bLastRightTrigger |= m_Gamepads[i].bLastRightTrigger;

                    if (m_Gamepads[i].bLeftTrigger != m_Gamepads[i].bLastLeftTrigger)
                        bActiveButtons[i] = TRUE;
                    if (m_Gamepads[i].bRightTrigger != m_Gamepads[i].bLastRightTrigger)
                        bActiveButtons[i] = TRUE;
                }
            }
        }

        // Clamp summed thumbstick values to proper range
        m_DefaultGamepad.sThumbLX = ClampToShort(iThumbLX);
        m_DefaultGamepad.sThumbLY = ClampToShort(iThumbLY);
        m_DefaultGamepad.sThumbRX = ClampToShort(iThumbRX);
        m_DefaultGamepad.sThumbRY = ClampToShort(iThumbRY);

        // Fill pActiveGamePadsMask
        if (pdwActiveGamePadsMask != NULL)
        {
            DWORD dwActiveGamePadsMask = 0;
            for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
            {
                if (bActiveButtons[i] || bActiveThumbs[i] || m_Gamepads[i].wPressedButtons)
                {
                    dwActiveGamePadsMask |= 1 << i;
                }
            }

            *pdwActiveGamePadsMask = dwActiveGamePadsMask;
        }

        // Assign an active gamepad
        for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
        {
            if (bActiveButtons[i])
            {
                m_DefaultGamepad.dwUserIndex = i;
                break;
            }

            if (bActiveThumbs[i])
            {
                m_DefaultGamepad.dwUserIndex = i;
                break;
            }
        }

        // Handle a convenient reboot sequence for all samples
        if ((m_DefaultGamepad.bLeftTrigger > 128) &&
            (m_DefaultGamepad.bRightTrigger > 128) &&
            (m_DefaultGamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER))
        {
            // Reboot the dev kit
            XLaunchNewImage("", 0);
        }

        return &m_DefaultGamepad;
    }

} // namespace ATG
