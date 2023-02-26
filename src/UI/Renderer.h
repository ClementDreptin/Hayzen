#pragma once

namespace Renderer
{

// Struct defined in the games.
struct Font_s
{
    int fontName;
    int pixelHeight;
    int glyphCount;
    int material;
    int glowMaterial;
    int glyphs;
};

typedef void (*R_ADDCMDDRAWSTRETCHPIC)(float x, float y, float w, float h, float s0, float t0, float s1, float t1, const float *color, HANDLE material);
typedef void (*R_ADDCMDDRAWTEXT)(const char *text, int maxChars, Font_s *font, float x, float y, float xScale, float yScale, float rotation, const float *color, int style);
typedef int (*R_TEXTWIDTH)(const char *text, int maxChars, Font_s *font);
typedef int (*R_TEXTHEIGHT)(Font_s *font);
typedef Font_s *(*R_REGISTERFONT)(const char *font, int imageTrack);
typedef HANDLE (*MATERIAL_REGISTERHANDLE)(const char *name, int imageTrack);

extern R_ADDCMDDRAWSTRETCHPIC R_AddCmdDrawStretchPic;
extern R_ADDCMDDRAWTEXT R_AddCmdDrawText;
extern R_TEXTWIDTH R_TextWidth;
extern R_TEXTHEIGHT R_TextHeight;
extern R_REGISTERFONT R_RegisterFont;
extern MATERIAL_REGISTERHANDLE Material_RegisterHandle;

extern Font_s *pFont;
extern HANDLE MaterialHandle;

extern float DisplayWidth;
extern float DisplayHeight;

float GetTextWidth(const std::string &text, float fontScale = 1.0f);
float GetTextHeight(float fontScale = 1.0f);

}

// Take a color and override its alpha channel.
#define D3DCOLOR_ALPHA(color, alpha) D3DCOLOR_ARGB(alpha, D3DCOLOR_GETRED(color), D3DCOLOR_GETGREEN(color), D3DCOLOR_GETBLUE(color))

#define CHAR_A "\x1"
#define CHAR_B "\x2"
#define CHAR_X "\x3"
#define CHAR_Y "\x4"
#define CHAR_LB "\x5"
#define CHAR_RB "\x6"
#define CHAR_START "\xE"
#define CHAR_BACK "\xF"
#define CHAR_LS "\x10"
#define CHAR_RS "\x11"
#define CHAR_LT "\x12"
#define CHAR_RT "\x13"
#define CHAR_UP "\x14"
#define CHAR_DOWN "\x15"
#define CHAR_LEFT "\x16"
#define CHAR_RIGHT "\x17"
