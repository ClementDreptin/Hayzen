#include "pch.h"
#include "Core\Plugin.h"

#include "Utils\Utils.h"
#include "Games\Games.h"

bool Plugin::Running = false;
DWORD Plugin::CurrentTitle = 0;

void Plugin::Start()
{
	Running = true;
	Utils::Thread((LPTHREAD_START_ROUTINE)MonitorTitleId);
}

void Plugin::Stop()
{
	Running = false;
}

DWORD Plugin::MonitorTitleId(LPVOID lpThreadParameter)
{
	while (Running)
	{
		DWORD newTitle = XamGetCurrentTitleId();
		if (newTitle != CurrentTitle)
			InitNewGame(newTitle);
	}

	return 0;
}

void Plugin::InitNewGame(DWORD titleId)
{
	CurrentTitle = titleId;

	switch (titleId)
	{
		case DASHBOARD:
			Dashboard::Init();
			break;
		case MW2:
			MW2::Init();
			break;
		default:
			return;
	}
}