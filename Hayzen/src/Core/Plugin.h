#pragma once

namespace Plugin
{
	extern bool Running;
	extern DWORD CurrentTitle;

	enum Games : DWORD
	{
		DASHBOARD = 0xFFFE07D1,
		MW2 = 0x41560817,
	};

	void Start();
	void Stop();

	DWORD MonitorTitleId(LPVOID lpThreadParameter);
	void InitNewGame(DWORD titleId);
}