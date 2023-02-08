#pragma once

struct Color
{
    float r;
    float g;
    float b;
    float a;
};

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

#define CHAR_UP "\x14"
#define CHAR_DOWN "\x15"
#define CHAR_X "\x3"
#define CHAR_LB "\x5"
#define CHAR_RB "\x6"
#define CHAR_RS "\x11"

class HudElem
{
public:
    static const Color s_ColorWhite;
    static const Color s_ColorBlack;
    static const float s_MenuX;
    static const float s_MenuY;
    static const float s_MenuWidth;
    static const float s_MenuHeight;
    static const float s_Padding;
    static const float s_TitleHeight;
    static const float s_LineHeight;

    static R_REGISTERFONT R_RegisterFont;
    static MATERIAL_REGISTERHANDLE Material_RegisterHandle;

    HudElem() {}

    HudElem(float x, float y, const Color &color);

    virtual ~HudElem() {}

    virtual void Draw() = 0;

    inline float GetX() const { return m_X; }

    inline float GetY() const { return m_Y; }

    inline const Color &GetColor() const { return m_Color; }

    inline void SetX(float x) { m_X = x; }

    inline void SetY(float y) { m_Y = y; }

    inline void SetColor(const Color &color) { m_Color = color; }

    inline void SetColor(float r, float g, float b, float a);

    inline void SetAlpha(float alpha) { m_Color.a = alpha; }

    inline static void SetFont(Font_s *pFont) { s_pFont = pFont; }

    inline static void SetMaterialHandle(HANDLE materialHandle) { s_MaterialHandle = materialHandle; }

    inline static void SetDrawTextFnPtr(R_ADDCMDDRAWTEXT drawTextFn) { R_AddCmdDrawText = drawTextFn; }

    inline static void SetDrawRectangleFnPtr(R_ADDCMDDRAWSTRETCHPIC drawRectangleFn) { R_AddCmdDrawStretchPic = drawRectangleFn; }

protected:
    float m_X;
    float m_Y;
    Color m_Color;

    static Font_s *s_pFont;
    static HANDLE s_MaterialHandle;

    static R_ADDCMDDRAWTEXT R_AddCmdDrawText;
    static R_ADDCMDDRAWSTRETCHPIC R_AddCmdDrawStretchPic;
};
