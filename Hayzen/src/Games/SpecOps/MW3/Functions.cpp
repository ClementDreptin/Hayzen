#include "pch.h"
#include "Games\SpecOps\MW3\Functions.h"

namespace SpecOps
{
namespace MW3
{
	void (*SV)(int clientNum, const char* text) = (void(*)(int, const char*))0x82371680;

	void (*R_AddCmdDrawText)(const char* text, int maxChars, Font_s* font, float x, float y, float xScale, float yScale, float rotation, const float* color, int style) =
		(void(*)(const char* , int, Font_s* , float, float, float, float, float, const float*, int))0x823F4E30;

	void (*R_AddCmdDrawStretchPic)(float x, float y, float w, float h, float s0, float t0, float s1, float t1, const float* color, void* material) =
		(void(*)(float, float, float, float, float, float, float, float, const float*, void*))0x823F4878;

	Font_s* (*R_RegisterFont)(const char* font, int imageTrack) = (Font_s*(*)(const char*, int))0x823DD130;

	void* (*Material_RegisterHandle)(const char* name, int imageTrack) = (void*(*)(const char *, int))0x823E95E8;

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

	void iPrintLn(int clientNum, const std::string& text)
	{
		SV(clientNum, Formatter::Format("gm \"%s\"", text.c_str()).c_str());
	}
}
}