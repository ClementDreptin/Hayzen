#include "pch.h"
#include "Core\Plugin.h"

#include "Games\Games.h"

namespace Plugin
{
    BOOL Running = FALSE;
    DWORD CurrentTitle = 0;

    enum Games
    {
        DASHBOARD = 0xFFFE07D1,
        MW2 = 0x41560817,
        MW3 = 0x415608CB
    };

    DWORD MonitorTitleId(LPVOID lpThreadParameter);

    VOID Start()
    {
        Running = TRUE;
        Memory::Thread((LPTHREAD_START_ROUTINE)MonitorTitleId);
    }

    VOID Stop()
    {
        Running = FALSE;
        Sleep(250);
    }

    VOID Cleanup()
    {
        MW2::SafeReset();
        Alpha::MW2::SafeReset();
        MW3::SafeReset();
        SpecOps::MW3::SafeReset();
        SpecOps::MW2::SafeReset();
    }

    VOID InitNewGame(DWORD titleId)
    {
        Cleanup();

        CurrentTitle = titleId;

        switch (titleId)
        {
            case DASHBOARD:
                Dashboard::Init();
                break;
            case MW2:
                if (!strcmp((LPSTR)0x82001270, "multiplayer"))
                    MW2::Init();
                else if (!strcmp((LPSTR)0x8200EFE4, "startMultiplayer"))
                    SpecOps::MW2::Init();
                else if (!strcmp((LPSTR)0x82001D38, "multiplayer"))
                    Alpha::MW2::Init();
                break;
            case MW3:
                if (!strcmp((LPSTR)0x82001458, "multiplayer"))
                    MW3::Init();
                else if (!strcmp((LPSTR)0x8200BEA8, "startMultiplayer"))
                    SpecOps::MW3::Init();
            default:
                return;
        }
    }

    DWORD MonitorTitleId(LPVOID lpThreadParameter)
    {
        while (Running)
        {
            DWORD newTitle = Kernel::XamGetCurrentTitleId();
            if (newTitle != CurrentTitle)
                InitNewGame(newTitle);
        }

        return 0;
    }
}