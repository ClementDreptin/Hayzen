#include "pch.h"
#include "Plugin.h"

#include "Games\Games.h"
#include "Utils\Memory.h"

void Plugin::Start()
{
	m_Running = true;
	Memory::Thread((LPTHREAD_START_ROUTINE)TitleIdMonitorThread);
	// TODO: try to call TitleIdMonitor on the same thread
}

void Plugin::Stop()
{
	m_Running = false;
}

DWORD Plugin::TitleIdMonitorThread(LPVOID pluginPtr)
{
	Plugin* plugin = (Plugin*)pluginPtr;
	plugin->TitleIdMonitor();

	return TRUE;
}

DWORD Plugin::TitleIdMonitor()
{
	while (m_Running)
	{
		if (XamGetCurrentTitleId() != m_CurrentGame)
			InitNewGame(XamGetCurrentTitleId());
	}

	return TRUE;
}

void Plugin::InitNewGame(DWORD titleId)
{
	m_CurrentGame = titleId;

	switch (titleId)
	{
		case Dashboard::s_TitleId:
			Dashboard::Init();
			break;
		case MW2::s_TitleId:
			if (!strcmp((char*)MW2::s_MPStringAddr, "multiplayer"))
				MW2::Init();
			break;
		default:
			break;
	}
}