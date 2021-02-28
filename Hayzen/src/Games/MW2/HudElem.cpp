#include "pch.h"
#include "Games\MW2\HudElem.h"

#include "Games\MW2\Functions.h"

namespace MW2
{
	void HudElem::SetShader(game_hudelem_s* elem, const char* materialName, float x, float y, int width, int height, int alignOrg, int alignScreen, float sort, byte r, byte g, byte b, byte a)
	{
		elem->elem.type = HE_TYPE_MATERIAL;
		elem->elem.alignOrg = alignOrg;
		elem->elem.alignScreen = alignScreen;
		elem->elem.x = x;
		elem->elem.y = y;
		elem->elem.color.r = r;
		elem->elem.color.g = g;
		elem->elem.color.b = b;
		elem->elem.color.a = a;
		elem->elem.width = width;
		elem->elem.height = height;
		elem->elem.sort = sort;
		elem->elem.materialIndex = G_MaterialIndex(materialName);
	}

	void HudElem::SetText(game_hudelem_s* elem, const char* text, int font, float fontScale, float x, float y, int alignOrg, int alignScreen, float sort, byte r, byte g, byte b, byte a)
	{
		elem->elem.type = HE_TYPE_TEXT;
		elem->elem.alignOrg = alignOrg;
		elem->elem.alignScreen = alignScreen;
		elem->elem.font = font;
		elem->elem.fontScale = fontScale;
		elem->elem.x = x;
		elem->elem.y = y;
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