#include "pch.h"
#include "Games\SpecOps\MW3\Functions.h"

using namespace XexUtils;

namespace SpecOps
{
namespace MW3
{
	void (*Cmd_RegisterNotification)(int clientNum, const char* notify, const char* command) = (void(*)(int, const char*, const char*))0x8228F278;

	const char* (*SL_ConvertToString)(unsigned int stringValue) = (const char*(*)(unsigned int))0x822B2338;

	void (*SV)(int clientNum, const char* text) = (void(*)(int, const char*))0x82371680;

	bool (*Dvar_GetBool)(const char* dvarName) = (bool(*)(const char*))0x82304420;

	int (*Dvar_GetInt)(const char* dvarName) = (int(*)(const char*))0x82304488;

	const char* (*Dvar_GetString)(const char* dvarName) = (const char*(*)(const char*))0x823045E0;

	void (*R_AddCmdDrawText)(const char* text, int maxChars, Font_s* font, float x, float y, float xScale, float yScale, float rotation, const float* color, int style) =
		(void(*)(const char* , int, Font_s* , float, float, float, float, float, const float*, int))0x823F4E30; //

	void (*R_AddCmdDrawStretchPic)(float x, float y, float w, float h, float s0, float t0, float s1, float t1, const float* color, void* material) =
		(void(*)(float, float, float, float, float, float, float, float, const float*, void*))0x823F4878; //

	Font_s* (*R_RegisterFont)(const char* font, int imageTrack) = (Font_s*(*)(const char*, int))0x823DD130; //

	void* (*Material_RegisterHandle)(const char* name, int imageTrack) = (void*(*)(const char *, int))0x823E95E8; //

	game_hudelem_s* (*HudElem_Alloc)(int clientNum) = (game_hudelem_s*(*)(int))0x822028C8;

	void (*HudElem_Free)(game_hudelem_s* hud) = (void(*)(game_hudelem_s*))0x82202A20;

	int (*G_MaterialIndex)(const char* name) = (int(*)(const char*))0x8223DF00;

	int (*G_FindLocalizedConfigstringIndex)(const char* string) = (int(*)(const char*))0x8223DCD8;

	playerState_s* (*SV_GetPlayerstateForClientNum)(int num) = (playerState_s*(*)(int))0x82371650;

	void (*TeleportPlayer)(gentity_s* player, float* origin, float* angles) = (void(*)(gentity_s*, float*, float*))0x8220C960;

	gclient_s* GetGClient(int clientNum)
	{
		return (gclient_s*)(0x83150090 + sizeof(gclient_s) * clientNum);
	}

	gentity_s* GetEntity(int entNum)
	{
		return (gentity_s*)(0x82F66560 + sizeof(gentity_s) * entNum);
	}

	void SetClientDvar(int clientNum, const std::string& dvar, const std::string& value)
	{
		SV(clientNum, Formatter::Format("setclientdvar \"%s\" \"%s\"", dvar.c_str(), value.c_str()).c_str());
	}

	void iPrintLn(int clientNum, const std::string& text)
	{
		SV(clientNum, Formatter::Format("gm \"%s\"", text.c_str()).c_str());
	}
}
}