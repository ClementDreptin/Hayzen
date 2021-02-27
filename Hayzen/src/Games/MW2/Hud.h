#pragma once

#include "Games\MW2\Structs.h"

namespace MW2
{
	void SetShader(game_hudelem_s* elem, const char* materialName, float x, float y, int width, int height, int alignOrg, int alignScreen, float sort = 0, byte r = 255, byte g = 255, byte b = 255, byte a = 255);

	void SetText(game_hudelem_s* elem, const char* text, int font, float fontScale, float x, float y, int alignOrg, int alignScreen, float sort = 1, byte r = 255, byte g = 255, byte b = 255, byte a = 255);
}