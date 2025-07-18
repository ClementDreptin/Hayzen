#pragma once

namespace UI
{

struct LineProps
{
    float X;
    float Y;
    float Width;
    float Height;
    D3DCOLOR Color;
};

void DrawLine(const LineProps &props);

typedef enum _BorderPosition
{
    Border_None = 0,
    Border_Left = 1 << 0,
    Border_Right = 1 << 1,
    Border_Top = 1 << 2,
    Border_Bottom = 1 << 3,
    Border_All = Border_Left | Border_Right | Border_Top | Border_Bottom,
} BorderPosition;

struct BorderProps
{
    float X;
    float Y;
    float Thickness;
    D3DCOLOR Color;
    BorderPosition Position;
    float Width;  // Width of the rectangle surrounded by the border.
    float Height; // Height of the rectangle surrounded by the border.
};

void DrawBorder(const BorderProps &props);

struct RectangleProps
{
    float X;
    float Y;
    float Width;
    float Height;
    D3DCOLOR Color;
    float BorderWidth;
    D3DCOLOR BorderColor;
    BorderPosition BorderPosition;
};

void DrawRectangle(const RectangleProps &props);

struct TextProps
{
    float X;
    float Y;
    std::string Text;
    float FontScale;
    D3DCOLOR Color;
    float BackgroundWidth;  // Use this if you want a different width than the one calculated to fit the string
    float BackgroundHeight; // Use this if you want a different height than the one calculated to fit the string
    D3DCOLOR BackgroundColor;
    float BorderWidth;
    D3DCOLOR BorderColor;
    BorderPosition BorderPosition;
};

void DrawText(const TextProps &props);

// Struct defined in the games
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
extern Font_s *pDefaultFont;
extern Font_s *pConsoleFont;
extern HANDLE MaterialHandle;

extern float DisplayWidth;
extern float DisplayHeight;

float GetTextWidth(const std::string &text, float fontScale = 1.0f);
float GetTextHeight(const std::string &text, float fontScale = 1.0f);
float GetFontHeight(float fontScale = 1.0f);

// Macros to override a specific channel of an existing color
#define D3DCOLOR_RED(color, red) D3DCOLOR_RGBA(red, D3DCOLOR_GETGREEN(color), D3DCOLOR_GETBLUE(color), D3DCOLOR_GETALPHA(color))
#define D3DCOLOR_GREEN(color, green) D3DCOLOR_RGBA(D3DCOLOR_GETRED(color), green, D3DCOLOR_GETBLUE(color), D3DCOLOR_GETALPHA(color))
#define D3DCOLOR_BLUE(color, blue) D3DCOLOR_RGBA(D3DCOLOR_GETRED(color), D3DCOLOR_GETGREEN(color), blue, D3DCOLOR_GETALPHA(color))
#define D3DCOLOR_ALPHA(color, alpha) D3DCOLOR_RGBA(D3DCOLOR_GETRED(color), D3DCOLOR_GETGREEN(color), D3DCOLOR_GETBLUE(color), alpha)

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

}
