#include "pch.h"
#include "Games\Alpha\MW2\HudElem.h"

#include "Games\Alpha\MW2\Functions.h"

namespace Alpha
{
namespace MW2
{
	const hudelem_color_t COLOR_WHITE = { 255, 255, 255, 255 };
	const hudelem_color_t COLOR_WHITE_NO_ALPHA = { 255, 255, 255, 0 };
	const hudelem_color_t COLOR_BLACK = { 0, 0, 0, 255 };
	const hudelem_color_t COLOR_BLACK_NO_ALPHA = { 0, 0, 0, 0 };

	void HudElem::SetShader(game_hudelem_s* elem, const char* materialName, int x, int y, int width, int height, hudelem_color_t color, float sort, align_t alignOrg, align_t alignScreen)
	{
		elem->elem.type = HE_TYPE_MATERIAL;
		elem->elem.alignOrg = alignOrg;
		elem->elem.alignScreen = alignScreen;
		elem->elem.x = (float)x;
		elem->elem.y = (float)y;
		elem->elem.color = color;
		elem->elem.width = width;
		elem->elem.height = height;
		elem->elem.sort = sort;
		elem->elem.materialIndex = G_MaterialIndex(materialName);
	}

	void HudElem::SetText(game_hudelem_s* elem, const char* text, float fontScale, int x, int y, hudelem_color_t color, int font, float sort, align_t alignOrg, align_t alignScreen)
	{
		elem->elem.type = HE_TYPE_TEXT;
		elem->elem.alignOrg = alignOrg;
		elem->elem.alignScreen = alignScreen;
		elem->elem.font = font;
		elem->elem.fontScale = fontScale;
		elem->elem.x = (float)x;
		elem->elem.y = (float)y;
		elem->elem.color = color;
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
}