#include "pch.h"
#include "Games\MW2\Functions.h"

void(_cdecl *MW2::Cmd_RegisterNotification)(int clientNum, const char* notify, const char* command) = (void(*)(int, const char*, const char*))0x82225858;

const char*(_cdecl *MW2::SL_ConvertToString)(unsigned int stringValue) = (const char*(*)(unsigned int))0x82241898;

void(_cdecl *MW2::SV)(int clientNum, int type, const char* text) = (void(*)(int, int, const char*))0x822548D8;

bool(_cdecl *MW2::Dvar_GetBool)(const char* DvarName) = (bool(*)(const char*))0x8229EEE8;