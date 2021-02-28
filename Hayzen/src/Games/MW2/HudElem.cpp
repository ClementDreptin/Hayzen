#include "pch.h"
#include "Games\MW2\HudElem.h"

#include "Games\MW2\Functions.h"

namespace MW2
{
	void HudElem::SetShader(game_hudelem_s* elem, const char* materialName, int x, int y, int width, int height, byte r, byte g, byte b, byte a, float sort, int alignOrg, int alignScreen)
	{
		elem->elem.type = HE_TYPE_MATERIAL;
		elem->elem.alignOrg = alignOrg;
		elem->elem.alignScreen = alignScreen;
		elem->elem.x = (float)x;
		elem->elem.y = (float)y;
		elem->elem.color.r = r;
		elem->elem.color.g = g;
		elem->elem.color.b = b;
		elem->elem.color.a = a;
		elem->elem.width = width;
		elem->elem.height = height;
		elem->elem.sort = sort;
		elem->elem.materialIndex = G_MaterialIndex(materialName);
	}

	void HudElem::SetText(game_hudelem_s* elem, const char* text, float fontScale, int x, int y, byte r, byte g, byte b, byte a, int font, float sort, int alignOrg, int alignScreen)
	{
		elem->elem.type = HE_TYPE_TEXT;
		elem->elem.alignOrg = alignOrg;
		elem->elem.alignScreen = alignScreen;
		elem->elem.font = font;
		elem->elem.fontScale = fontScale;
		elem->elem.x = (float)x;
		elem->elem.y = (float)y;
		elem->elem.color.r = r;
		elem->elem.color.g = g;
		elem->elem.color.b = b;
		elem->elem.color.a = a;
		elem->elem.sort = sort;
		elem->elem.text = G_LocalizedStringIndex(text);
	}

	void HudElem::MakeDisappear(game_hudelem_s* elem)
	{
		elem->elem.color.a = 0;
	}

	void HudElem::MakeAppear(game_hudelem_s* elem, byte alpha)
	{
		elem->elem.color.a = alpha;
	}
}