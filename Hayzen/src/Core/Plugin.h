#pragma once

class Plugin
{
public:
	Plugin() {};

	void Start();
	void Stop();

	static DWORD TitleIdMonitorThread(LPVOID pluginPtr);
private:
	bool m_Running;
	DWORD m_CurrentGame;

	DWORD TitleIdMonitor();

	void InitNewGame(DWORD titleId);
};