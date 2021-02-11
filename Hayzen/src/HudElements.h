#include "pch.h"
#include "functions.h"

struct game_hudelem_s;

game_hudelem_s* (__cdecl *HudElem_Alloc)(int clientIndex, int TeamNum) = (game_hudelem_s*(*)(int, int))0x821DF928;//

int(__cdecl *G_LocalizedStringIndex)(const char* String) = (int(*)(const char*))0x8220C7A0;//

int(__cdecl *G_MaterialIndex)(const char* Name) = (int(*)(const char*))0x8220C960;//

struct hudelem_color_t
{
	byte r;
	byte g;
	byte b;
	byte a;
};
 
struct hudelem_s
{
	int type;                     // 0x00-0x04
	float y;                      // 0x04-0x08
	float x;                      // 0x08-0x0C
	float z;                      // 0x0C-0x10
	int targetEntNum;             // 0x10-0x14
	float fontScale;              // 0x14-0x18
	float fromFontScale;          // 0x18-0x1C
	int fontScaleStartTime;       // 0x1C-0x20
	int fontScaleTime;            // 0x20-0x24
	int label;                    // 0x24-0x28
	int font;                     // 0x28-0x2C
	int alignOrg;                 // 0x2C-0x30
	int alignScreen;              // 0x30-0x34
	hudelem_color_t color;        // 0x34-0x38
	hudelem_color_t fromColor;    // 0x38-0x3C
	int fadeStartTime;            // 0x3C-0x40
	int fadeTime;                 // 0x40-0x44
	int height;                   // 0x44-0x48
	int width;                    // 0x48-0x4C
	int materialIndex;            // 0x4C-0x50
	int fromHeight;               // 0x50-0x54
	int fromWidth;                // 0x54-0x58
	int scaleStartTime;           // 0x58-0x5C
	int scaleTime;                // 0x5C-0x60
	float fromY;                  // 0x60-0x64
	float fromX;                  // 0x64-0x68
	int fromAlignOrg;             // 0x68-0x6C
	int fromAlignScreen;          // 0x6C-0x70
	int moveStartTime;            // 0x70-0x74
	int moveTime;                 // 0x74-0x78
	float value;                  // 0x78-0x7C
	int time;                     // 0x7C-0x80
	int duration;                 // 0x80-0x84
	int text;                     // 0x84-0x88
	float sort;                   // 0x88-0x8C
	hudelem_color_t glowColor;    // 0x8C-0x90
	int fxBirthTime;              // 0x90-0x94
	int fxLetterTime;             // 0x94-0x98
	int fxDecayStartTime;         // 0x98-0x9C
	int fxDecayDuration;          // 0x9C-0xA0
	int soundID;                  // 0xA0-0xA4
	int flags;                    // 0xA4-0xA8
};
 
struct game_hudelem_s
{
	hudelem_s elem;               // 0x00-0xA8
	int clientNum;                // 0xA8-0xAC
	int teamNum;                  // 0xAC-0xB0
	int archived;                 // 0xB0-0xB4
};

game_hudelem_s * spawnAllClientElem()
{
	game_hudelem_s * elem = HudElem_Alloc(0, 0);
	elem->clientNum = 0x7FF;
	return elem;
}
 
void setText(game_hudelem_s * elem, const char * text, int font, float fontScale, float x, float y, int alignOrg, float sort, byte r = 255, byte g = 255, byte b = 255, byte a = 255)
{
	elem->elem.type = 0x01;
	elem->elem.alignOrg = alignOrg;
	elem->elem.alignScreen = 0x01;
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
 
void setShader(game_hudelem_s * elem, const char * materialName, float x, float y, int width, int height, int alignOrg, float sort, byte r = 255, byte g = 255, byte b = 255, byte a = 255)
{
	elem->elem.type = 0x04;
	elem->elem.alignOrg = alignOrg;
	elem->elem.alignScreen = 0x01;
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

void MoveOverTime(game_hudelem_s *elem, int Time, float X, float Y, int delay = 0)
{
	elem->elem.fromX = elem->elem.x;
	elem->elem.fromY = elem->elem.y;
	elem->elem.fromAlignOrg = elem->elem.alignOrg;
	elem->elem.fromAlignScreen = elem->elem.alignScreen;
	elem->elem.moveStartTime = *(int*)0x8310A12C + delay;
	elem->elem.moveTime = Time;
	elem->elem.x = X;
	elem->elem.y = Y;
}

void scaleOverTime(game_hudelem_s *elem, int Time, int Width, int Height, int delay = 0)
{
	elem->elem.scaleTime = Time;
	elem->elem.scaleStartTime = *(int*)0x8310A12C + delay;
	elem->elem.fromHeight = elem->elem.height;
	elem->elem.fromWidth = elem->elem.width;
	elem->elem.width = Width;
	elem->elem.height = Height;
}

void FontScaleOverTime(game_hudelem_s *elem, int Time, float Scale, int delay = 0)
{
	elem->elem.fromFontScale = elem->elem.fontScale;
	elem->elem.fontScaleStartTime = *(int*)0x8310A12C + 0;
	elem->elem.fontScaleTime = Time;
	elem->elem.fontScale = Scale;
}

void FadeOverTime(game_hudelem_s *elem, int Time, byte Red, byte Green, byte Blue, byte Alpha, int delay = 0)
{
	elem->elem.fromColor = elem->elem.color;
	elem->elem.fadeStartTime = *(int*)0x8310A12C + delay;
	elem->elem.fadeTime = Time;
	elem->elem.color.r = Red;
	elem->elem.color.g = Green;
	elem->elem.color.b = Blue;
	elem->elem.color.a = Alpha;
}

void ChangeOptionText(game_hudelem_s * Elem, const char* Text, int delay = 0)
{
	Elem->elem.color.r = 0;
	Elem->elem.color.g = 0;
	Elem->elem.color.b = 0;
	Elem->elem.color.a = 0;
	Elem->elem.text = G_LocalizedStringIndex(Text);
	FadeOverTime(Elem, 350, 255, 255, 255, 255, delay);
}