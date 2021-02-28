#include "pch.h"
#include "Games\MW2\Functions.h"

#include "Games\MW2\Structs.h"

void(__cdecl *MW2::Cmd_RegisterNotification)(int clientNum, const char* notify, const char* command) = (void(*)(int, const char*, const char*))0x82225858;

const char*(__cdecl *MW2::SL_ConvertToString)(unsigned int stringValue) = (const char*(*)(unsigned int))0x82241898;

void(__cdecl *MW2::SV)(int clientNum, int type, const char* text) = (void(*)(int, int, const char*))0x822548D8;

bool(__cdecl *MW2::Dvar_GetBool)(const char* DvarName) = (bool(*)(const char*))0x8229EEE8;

MW2::game_hudelem_s*(__cdecl *MW2::HudElem_Alloc)(int clientNum, int teamNum) = (MW2::game_hudelem_s*(*)(int, int))0x821DF928;

void(__cdecl *MW2::HudElem_Free)(MW2::game_hudelem_s* hud) = (void(*)(MW2::game_hudelem_s*))0x821DF9C0;

int(__cdecl *MW2::G_MaterialIndex)(const char* name) = (int(*)(const char*))0x8220C960;

int(__cdecl *MW2::G_LocalizedStringIndex)(const char* string) = (int(*)(const char*))0x8220C7A0;

MW2::clientState_s*(__cdecl *MW2::GetClientState)(int clientNum) = (MW2::clientState_s*(*)(int))0x821E6610;