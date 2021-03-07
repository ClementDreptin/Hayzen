#include "pch.h"
#include "Games\MW2\Functions.h"

#include "Games\MW2\Structs.h"
#include "Utils\Formatter.h"

void(__cdecl *MW2::Cmd_RegisterNotification)(int clientNum, const char* notify, const char* command) = (void(*)(int, const char*, const char*))0x82225858;

const char*(__cdecl *MW2::SL_ConvertToString)(unsigned int stringValue) = (const char*(*)(unsigned int))0x82241898;

void(__cdecl *MW2::SV)(int clientNum, int type, const char* text) = (void(*)(int, int, const char*))0x822548D8;

bool(__cdecl *MW2::Dvar_GetBool)(const char* dvarName) = (bool(*)(const char*))0x8229EEE8;

int(__cdecl *MW2::Dvar_GetInt)(const char* dvarName) = (int(*)(const char*))0x8229EF50;

MW2::game_hudelem_s*(__cdecl *MW2::HudElem_Alloc)(int clientNum, int teamNum) = (MW2::game_hudelem_s*(*)(int, int))0x821DF928;

void(__cdecl *MW2::HudElem_Free)(MW2::game_hudelem_s* hud) = (void(*)(MW2::game_hudelem_s*))0x821DF9C0;

int(__cdecl *MW2::G_MaterialIndex)(const char* name) = (int(*)(const char*))0x8220C960;

int(__cdecl *MW2::G_LocalizedStringIndex)(const char* string) = (int(*)(const char*))0x8220C7A0;

MW2::clientState_s*(__cdecl *MW2::GetClientState)(int clientNum) = (MW2::clientState_s*(*)(int))0x821E6610;

MW2::playerState_s*(__cdecl *MW2::GetPlayerState)(int clientNum) = (MW2::playerState_s*(*)(int))0x821E6628;

MW2::gclient_s* MW2::GetGClient(int clientNum)
{
	return (gclient_s*)(0x830CBF80 + sizeof(gclient_s) * clientNum);
}

void MW2::SetClientDvar(int clientNum, const std::string& dvar, const std::string& value)
{
	SV(clientNum, 0, Formatter::Format("s %s \"%s\"", dvar.c_str(), value.c_str()).c_str());
}

void MW2::iPrintLn(int clientNum, const std::string& text)
{
	SV(clientNum, 0, Formatter::Format("f \"%s\"", text.c_str()).c_str());
}