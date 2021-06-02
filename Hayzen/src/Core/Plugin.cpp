#include "pch.h"
#include "Core\Plugin.h"

#include "Games\Games.h"

bool Plugin::Running = false;
DWORD Plugin::CurrentTitle = 0;

void Plugin::Start()
{
	Running = true;
	Memory::Thread((LPTHREAD_START_ROUTINE)MonitorTitleId);
}

void Plugin::Stop()
{
	Running = false;
}

DWORD Plugin::MonitorTitleId(LPVOID lpThreadParameter)
{
	while (Running)
	{
		DWORD newTitle = Kernel::XamGetCurrentTitleId();
		if (newTitle != CurrentTitle)
			InitNewGame(newTitle);
	}

	return 0;
}

void Plugin::InitNewGame(DWORD titleId)
{
	Cleanup();

	CurrentTitle = titleId;

	switch (titleId)
	{
		case DASHBOARD:
			Dashboard::Init();
			break;
		case MW2:
			if (!strcmp((char*)0x82001270, "multiplayer"))
				MW2::Init();
			else if (!strcmp((char*)0x82001D38, "multiplayer"))
				Alpha::MW2::Init();
			break;
		case MW3:
			if (!strcmp((char*)0x82001458, "multiplayer"))
				MW3::Init();
			else if (!strcmp((char*)0x8200BEA8, "startMultiplayer"))
				SpecOps::MW3::Init();
		default:
			return;
	}
}

void Plugin::Cleanup()
{
	MW2::SafeReset();
	Alpha::MW2::SafeReset();
	MW3::SafeReset();
	SpecOps::MW3::SafeReset();
}