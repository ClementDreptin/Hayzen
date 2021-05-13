#include "pch.h"
#include "Games\MW3\Functions.h"

#include "Games\MW3\Structs.h"

using namespace XexUtils;

namespace MW3
{
	void (*Cmd_RegisterNotification)(int clientNum, const char* notify, const char* command) = (void(*)(int, const char*, const char*))0x82288FF0;

	const char* (*SL_ConvertToString)(unsigned int stringValue) = (const char*(*)(unsigned int))0x822B5120;

	void (*SV)(int clientNum, int type, const char* text) = (void(*)(int, int, const char*))0x822C9340;

	bool (*Dvar_GetBool)(const char* dvarName) = (bool(*)(const char*))0x8232E2C8;

	int (*Dvar_GetInt)(const char* dvarName) = (int(*)(const char*))0x8232E330;

	const char* (*Dvar_GetString)(const char* dvarName) = (const char*(*)(const char*))0x8232E488;

	game_hudelem_s* (*HudElem_Alloc)(int clientNum, int teamNum) = (game_hudelem_s*(*)(int, int))0x8223D260;

	void (*HudElem_Free)(game_hudelem_s* hud) = (void(*)(game_hudelem_s*))0x8223D390;

	int (*G_MaterialIndex)(const char* name) = (int(*)(const char*))0x8226E180;

	int (*G_LocalizedStringIndex)(const char* string) = (int(*)(const char*))0x8226E0A0;

	clientState_s* (*GetClientState)(int clientNum) = (clientState_s*(*)(int))0x82244130;

	playerState_s* (*GetPlayerState)(int clientNum) = (playerState_s*(*)(int))0x82244148;

	bool (*Session_IsHost)(DWORD sessionDataPtr, int clientNum) = (bool(*)(DWORD, int))0x823BFE78;

	void (*SP_script_model)(gentity_s* mSelf) = (void(*)(gentity_s*))0x82268138;

	gentity_s*(*G_Spawn)() = (gentity_s*(*)())0x8226F378;

	void (*G_SetModel)(gentity_s* ent, const char* modelName) = (void(*)(gentity_s*, const char*))0x8226EAA0;

	void (*SV_LinkEntity)(gentity_s* gEnt) = (void(*)(gentity_s*))0x822D6E78;

	void (*SV_UnlinkEntity)(gentity_s* gEnt) = (void(*)(gentity_s*))0x822D6D90;

	void (*SV_SetBrushModel)(gentity_s* ent) = (void(*)(gentity_s*))0x822C95C8;

	void (*TeleportPlayer)(gentity_s* player, float* origin, float* angles) = (void(*)(gentity_s*, float*, float*))0x82246048;

	gclient_s* GetGClient(int clientNum)
	{
		return (gclient_s*)(0x82F99580 + sizeof(gclient_s) * clientNum);
	}

	gentity_s* GetEntity(int entNum)
	{
		return (gentity_s*)(0x82DCCC80 + sizeof(gentity_s) * entNum);
	}

	void SetClientDvar(int clientNum, const std::string& dvar, const std::string& value)
	{
		SV(clientNum, 0, Formatter::Format("q %s \"%s\"", dvar.c_str(), value.c_str()).c_str());
	}

	void iPrintLn(int clientNum, const std::string& text)
	{
		SV(clientNum, 0, Formatter::Format("f \"%s\"", text.c_str()).c_str());
	}

	bool IsHost(int clientNum)
	{
		return Session_IsHost(0x83BC0148, clientNum);
	}

	gentity_s* GetCurrentMapBrushModel()
	{
		std::string mapName = Dvar_GetString("ui_mapname");

		if (mapName == "mp_seatown")
			return (gentity_s*)0x82DD1280;
		if (mapName == "mp_mogadishu")
			return (gentity_s*)0x82E08A00;
		if (mapName == "mp_exchange")
			return (gentity_s*)0x82E14580;
		if (mapName == "mp_radar")
			return (gentity_s*)0x82DD3A80;
		if (mapName == "mp_terminal_cls")
			return (gentity_s*)0x82DF9C80;
		else
			return (gentity_s*)0x82DD1500;
	}
}