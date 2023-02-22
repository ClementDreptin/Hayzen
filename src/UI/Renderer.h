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

typedef void (*R_ADDCMDDRAWTEXT)(const char *text, int maxChars, Font_s *font, float x, float y, float xScale, float yScale, float rotation, const float *color, int style);
typedef void (*R_ADDCMDDRAWSTRETCHPIC)(float x, float y, float w, float h, float s0, float t0, float s1, float t1, const float *color, HANDLE material);
typedef Font_s *(*R_REGISTERFONT)(const char *font, int imageTrack);
typedef HANDLE (*MATERIAL_REGISTERHANDLE)(const char *name, int imageTrack);

extern R_ADDCMDDRAWSTRETCHPIC R_AddCmdDrawStretchPic;
extern R_ADDCMDDRAWTEXT R_AddCmdDrawText;
extern R_REGISTERFONT R_RegisterFont;
extern MATERIAL_REGISTERHANDLE Material_RegisterHandle;

extern Font_s *pFont;
extern HANDLE MaterialHandle;

}

#define CHAR_UP "\x14"
#define CHAR_DOWN "\x15"
#define CHAR_X "\x3"
#define CHAR_LB "\x5"
#define CHAR_RB "\x6"
#define CHAR_RS "\x11"
