#pragma once

class MW2
{
public:
	static const DWORD s_MPStringAddr = 0x82001270;
	static const DWORD s_TitleId = 0x41560817;

	static void Init();
private:
	static const DWORD s_XamLoaderTerminateTitleAddr = 0x823B6534;
	static const DWORD s_XamInputGetStateAddr = 0x823B64C4;
	static const DWORD s_SCR_DrawScreenFieldAddr = 0x8214BEB8;
	static const DWORD s_CL_ReadyToSendPacketAddr = 0x821401E0;
	static const DWORD s_R_AddCmdDrawStretchPicAddr = 0x8234F9B8;
	static const DWORD s_Cmd_TokenizeStringAddr = 0x822252B0;
	static const DWORD s_CL_ConsolePrintAddr = 0x8213BF40;

	//static void XamLoaderTerminateTitle_Hook();
	//static DWORD XamInputGetState_Hook(DWORD userIndex, DWORD flags, PXINPUT_STATE pState);
	//static void SCR_DrawScreenField_Hook(const int localClientNum, int refreshedUI);

	MW2() {}
};