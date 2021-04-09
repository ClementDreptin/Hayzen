#include "pch.h"
#include "Games\MW2\Functions.h"

#include "Games\MW2\Structs.h"

namespace MW2
{
	void (*Cmd_RegisterNotification)(int clientNum, const char* notify, const char* command) = (void(*)(int, const char*, const char*))0x82225858;

	const char* (*SL_ConvertToString)(unsigned int stringValue) = (const char*(*)(unsigned int))0x82241898;

	void (*SV)(int clientNum, int type, const char* text) = (void(*)(int, int, const char*))0x822548D8;

	bool (*Dvar_GetBool)(const char* dvarName) = (bool(*)(const char*))0x8229EEE8;

	int (*Dvar_GetInt)(const char* dvarName) = (int(*)(const char*))0x8229EF50;

	const char* (*Dvar_GetString)(const char* dvarName) = (const char*(*)(const char*))0x8229F0A8;

	game_hudelem_s* (*HudElem_Alloc)(int clientNum, int teamNum) = (game_hudelem_s*(*)(int, int))0x821DF928;

	void (*HudElem_Free)(game_hudelem_s* hud) = (void(*)(game_hudelem_s*))0x821DF9C0;

	int (*G_MaterialIndex)(const char* name) = (int(*)(const char*))0x8220C960;

	int (*G_LocalizedStringIndex)(const char* string) = (int(*)(const char*))0x8220C7A0;

	clientState_s* (*GetClientState)(int clientNum) = (clientState_s*(*)(int))0x821E6610;

	playerState_s* (*GetPlayerState)(int clientNum) = (playerState_s*(*)(int))0x821E6628;

	bool (*Session_IsHost)(DWORD sessionDataPtr, int clientNum) = (bool(*)(DWORD, int))0x82320138;

	void (*SP_script_model)(gentity_s* mSelf) = (void(*)(gentity_s*))0x82206D88;

	gentity_s*(*G_Spawn)() = (gentity_s*(*)())0x8220DB50;

	void (*G_SetModel)(gentity_s* ent, const char* modelName) = (void(*)(gentity_s*, const char*))0x8220D278;

	void (*SV_LinkEntity)(gentity_s* gEnt) = (void(*)(gentity_s*))0x8225F518;

	void (*SV_UnlinkEntity)(gentity_s* gEnt) = (void(*)(gentity_s*))0x8225F430;

	void (*SV_SetBrushModel)(gentity_s* ent) = (void(*)(gentity_s*))0x82254B50;

	gentity_s* (*SV_AddTestClient)() = (gentity_s*(*)())0x82254690;

	void (*SV_ExecuteClientCommand)(int client, const char* s, int clientOK, int fromOldServer) = (void(*)(int, const char*, int, int))0x82253140;

	void (*TeleportPlayer)(gentity_s* player, float* origin, float* angles) = (void(*)(gentity_s*, float*, float*))0x821E8198;

	gclient_s* GetGClient(int clientNum)
	{
		return (gclient_s*)(0x830CBF80 + sizeof(gclient_s) * clientNum);
	}

	gentity_s* GetEntity(int entNum)
	{
		return (gentity_s*)(0x82F03600 + sizeof(gentity_s) * entNum);
	}

	void SetClientDvar(int clientNum, const std::string& dvar, const std::string& value)
	{
		SV(clientNum, 0, XexUtils::Formatter::Format("s %s \"%s\"", dvar.c_str(), value.c_str()).c_str());
	}

	void iPrintLn(int clientNum, const std::string& text)
	{
		SV(clientNum, 0, XexUtils::Formatter::Format("f \"%s\"", text.c_str()).c_str());
	}

	bool IsHost(int clientNum)
	{
		return Session_IsHost(0x83AC3DB0, clientNum);
	}

	gentity_s* GetCurrentMapBrushModel()
	{
		std::string mapName = Dvar_GetString("ui_mapname");

		if (mapName == "mp_afghan")
			return (gentity_s*)0x82F7E800;
		if (mapName == "mp_derail")
			return (gentity_s*)0x82F5F680;
		if (mapName == "mp_estate")
			return (gentity_s*)0x82F4AF00;
		if (mapName == "mp_favela")
			return (gentity_s*)0x82F70C00;
		if (mapName == "mp_highrise")
			return (gentity_s*)0x82F76380;
		if (mapName == "mp_invasion")
			return (gentity_s*)0x82F73180;
		if (mapName == "mp_checkpoint")
			return (gentity_s*)0x82F6D280;
		if (mapName == "mp_quarry")
			return (gentity_s*)0x82F99600;
		if (mapName == "mp_rundown")
			return (gentity_s*)0x82F65800;
		if (mapName == "mp_rust")
			return (gentity_s*)0x82F3C900;
		if (mapName == "mp_boneyard")
			return (gentity_s*)0x82F0B300;
		if (mapName == "mp_nightshift")
			return (gentity_s*)0x82F3A880;
		if (mapName == "mp_subbase")
			return (gentity_s*)0x82F59780;
		if (mapName == "mp_terminal")
			return (gentity_s*)0x82F0DD80;
		if (mapName == "mp_underpass")
			return (gentity_s*)0x82F66C00;
		if (mapName == "mp_brecourt")
			return (gentity_s*)0x82F15580;
		else
			return nullptr;
	}
}